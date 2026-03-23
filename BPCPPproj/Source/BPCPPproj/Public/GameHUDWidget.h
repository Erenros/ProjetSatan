#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUDWidget.generated.h"

class UCollectibleGameInstance;

UCLASS()
class BPCPPPROJ_API UGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	// ===== TIMER =====

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Timer")
	int32 TotalSeconds = 0;

	UPROPERTY(BlueprintReadOnly, Category = "HUD|Timer")
	FString FormattedTime = "00:00";

	// Event BP pour update l'UI du timer
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD|Timer")
	void OnTimerUpdated(const FString& TimeString, int32 Minutes, int32 Seconds);

	// Contrôle du timer
	UFUNCTION(BlueprintCallable, Category = "HUD|Timer")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category = "HUD|Timer")
	void StopTimer();

	UFUNCTION(BlueprintCallable, Category = "HUD|Timer")
	void ResetTimer();

	UFUNCTION(BlueprintCallable, Category = "HUD|Timer")
	void PauseTimer();

	UFUNCTION(BlueprintCallable, Category = "HUD|Timer")
	void ResumeTimer();

	// Getters pour le timer
	UFUNCTION(BlueprintPure, Category = "HUD|Timer")
	int32 GetTotalSeconds() const { return TotalSeconds; }

	UFUNCTION(BlueprintPure, Category = "HUD|Timer")
	FString GetFormattedTime() const { return FormattedTime; }

	// ===== COLLECTIBLES =====

	// Event BP appelé quand un collectible change
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD|Collectibles")
	void OnCollectibleUpdated(FName CollectibleType, int32 NewCount);

	// Getters pour les collectibles
	UFUNCTION(BlueprintPure, Category = "HUD|Collectibles")
	int32 GetCollectibleCount(FName CollectibleType) const;

	UFUNCTION(BlueprintPure, Category = "HUD|Collectibles")
	int32 GetTotalCollectibles() const;

	// Force un refresh de tous les collectibles
	UFUNCTION(BlueprintCallable, Category = "HUD|Collectibles")
	void RefreshAllCollectibles();

private:
	FTimerHandle TimerHandle;
	bool bIsTimerPaused = false;

	UPROPERTY()
	UCollectibleGameInstance* CachedGameInstance;

	void UpdateTimer();

	UFUNCTION()
	void OnCollectibleCountChanged(FName CollectibleType, int32 NewCount);
};