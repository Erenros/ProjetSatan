#include "GameHUDWidget.h"
#include "CollectibleGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void UGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartTimer();

	CachedGameInstance = Cast<UCollectibleGameInstance>(UGameplayStatics::GetGameInstance(this));

	if (CachedGameInstance)
	{
		// S'abonner aux changements de collectibles
		CachedGameInstance->OnCollectibleCountChanged.AddDynamic(this, &UGameHUDWidget::OnCollectibleCountChanged);

		RefreshAllCollectibles();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameHUDWidget: CollectibleGameInstance not found!"));
	}
}

void UGameHUDWidget::NativeDestruct()
{
	// Nettoyer le timer
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	// Se désabonner des events
	if (CachedGameInstance)
	{
		CachedGameInstance->OnCollectibleCountChanged.RemoveDynamic(this, &UGameHUDWidget::OnCollectibleCountChanged);
	}

	Super::NativeDestruct();
}

// ===== TIMER FUNCTIONS =====

void UGameHUDWidget::StartTimer()
{
	if (!GetWorld()) return;

	TotalSeconds = 0;
	bIsTimerPaused = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGameHUDWidget::UpdateTimer, 1.0f, true);
	UpdateTimer();
}

void UGameHUDWidget::StopTimer()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void UGameHUDWidget::ResetTimer()
{
	TotalSeconds = 0;
	UpdateTimer();
}

void UGameHUDWidget::PauseTimer()
{
	if (GetWorld() && !bIsTimerPaused)
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
		bIsTimerPaused = true;
	}
}

void UGameHUDWidget::ResumeTimer()
{
	if (GetWorld() && bIsTimerPaused)
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TimerHandle);
		bIsTimerPaused = false;
	}
}

void UGameHUDWidget::UpdateTimer()
{
	TotalSeconds++;

	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;

	// Format: 06:66
	FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

	// Broadcast l'event BP pour update l'UI
	OnTimerUpdated(FormattedTime, Minutes, Seconds);
}

// ===== COLLECTIBLES FUNCTIONS =====

void UGameHUDWidget::OnCollectibleCountChanged(FName CollectibleType, int32 NewCount)
{
	UE_LOG(LogTemp, Log, TEXT("HUD: %s = %d"), *CollectibleType.ToString(), NewCount);

	// Broadcast l'event BP pour update l'UI
	OnCollectibleUpdated(CollectibleType, NewCount);
}

int32 UGameHUDWidget::GetCollectibleCount(FName CollectibleType) const
{
	if (CachedGameInstance)
	{
		return CachedGameInstance->GetCollectibleCount(CollectibleType);
	}
	return 0;
}

int32 UGameHUDWidget::GetTotalCollectibles() const
{
	if (CachedGameInstance)
	{
		return CachedGameInstance->GetTotalCollectibles();
	}
	return 0;
}

void UGameHUDWidget::RefreshAllCollectibles()
{
	if (!CachedGameInstance) return;

	// Récupérer tous les collectibles et forcer un update de l'UI
	TMap<FName, int32> AllCollectibles = CachedGameInstance->GetAllCollectibles();

	for (const auto& Pair : AllCollectibles)
	{
		OnCollectibleUpdated(Pair.Key, Pair.Value);
	}
}