#include "BaseCollectible.h"
#include "Components/BoxComponent.h"
#include "AudioManagerSubsystem.h"
#include "CollectibleGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"

ABaseCollectible::ABaseCollectible()
{
	// Par défaut, les collectibles ne peuvent être ramassés qu'une fois
	bCanInteractMultipleTimes = false;
}

void ABaseCollectible::BeginPlay()
{
	Super::BeginPlay();

	// Les overlaps sont déjà bindés dans la classe parente
	UE_LOG(LogTemp, Log, TEXT("[BaseCollectible] %s initialized - Type: %s, Amount: %d"),
		*GetName(), *CollectibleType.ToString(), CollectibleAmount);
}

void ABaseCollectible::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Appeler la version parent pour mettre à jour CurrentOverlappingActor
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// Si c'est un personnage et que l'auto-collect est activé
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		if (bAutoCollectOnOverlap && !bAlreadyCollected)
		{
			UE_LOG(LogTemp, Log, TEXT("[BaseCollectible] Auto-collecting %s"), *GetName());
			Execute_OnInteract(this, OtherActor);
		}
	}
}

void ABaseCollectible::OnInteract_Implementation(AActor* InstigatorActor)
{
	// Éviter la double collection
	if (bAlreadyCollected)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BaseCollectible] Already collected: %s"), *GetName());
		return;
	}

	// Marquer comme collecté immédiatement
	bAlreadyCollected = true;

	UE_LOG(LogTemp, Log, TEXT("[BaseCollectible] Collecting %s (Type: %s, Amount: %d)"),
		*GetName(), *CollectibleType.ToString(), CollectibleAmount);

	// Appeler la fonction d'interface
	Execute_OnCollected(this, InstigatorActor);
}

void ABaseCollectible::OnCollected_Implementation(AActor* Collector)
{
	// Jouer le son de collection
	if (CollectionSound)
	{
		if (UAudioManagerSubsystem* AudioSub = GetWorld()->GetSubsystem<UAudioManagerSubsystem>())
		{
			AudioSub->PlaySoundAtLocation(CollectionSound, GetActorLocation(),
				CollectionSoundVolume, CollectionSoundPitch);
		}
	}
	else if (InteractionSound) // Fallback sur le son d'interaction de base
	{
		if (UAudioManagerSubsystem* AudioSub = GetWorld()->GetSubsystem<UAudioManagerSubsystem>())
		{
			AudioSub->PlaySoundAtLocation(InteractionSound, GetActorLocation(),
				InteractionSoundVolume, InteractionSoundPitch);
		}
	}

	// Ajouter le collectible au GameInstance
	if (UCollectibleGameInstance* GameInstance = Cast<UCollectibleGameInstance>(GetGameInstance()))
	{
		GameInstance->AddCollectible(CollectibleType, CollectibleAmount);
		UE_LOG(LogTemp, Log, TEXT("[BaseCollectible] Added to GameInstance - Type: %s, Amount: %d"),
			*CollectibleType.ToString(), CollectibleAmount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BaseCollectible] CollectibleGameInstance not found! Set it in Project Settings → Game Instance Class"));
	}

	// Event Blueprint pour VFX personnalisés
	OnCollectedEvent(Collector);

	// Gestion de la destruction
	HandleDestruction();
}

void ABaseCollectible::HandleDestruction()
{
	if (!bDestroyOnCollect)
	{
		// Désactiver la collision pour éviter re-collection
		InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Log, TEXT("[BaseCollectible] %s - Collision disabled (not destroying)"), *GetName());
		return;
	}

	if (DestroyDelay > 0.0f)
	{
		// Masquer l'acteur et désactiver collision immédiatement
		SetActorHiddenInGame(true);
		InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Destruction avec délai pour laisser le son/VFX jouer
		FTimerHandle DestroyTimer;
		GetWorldTimerManager().SetTimer(DestroyTimer, [this]()
			{
				UE_LOG(LogTemp, Log, TEXT("[BaseCollectible] Destroying %s"), *GetName());
				Destroy();
			}, DestroyDelay, false);
	}
	else
	{
		// Destruction immédiate
		UE_LOG(LogTemp, Log, TEXT("[BaseCollectible] Destroying %s immediately"), *GetName());
		Destroy();
	}
}