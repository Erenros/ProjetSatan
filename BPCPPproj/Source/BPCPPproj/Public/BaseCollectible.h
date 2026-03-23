#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "CollectibleInterface.h"
#include "BaseCollectible.generated.h"

/**
 * Classe de base pour tous les collectibles
 * Hérite de BaseInteractable pour la détection
 * Implémente CollectibleInterface pour la logique de collection
 */
UCLASS()
class BPCPPPROJ_API ABaseCollectible : public ABaseInteractable, public ICollectibleInterface
{
	GENERATED_BODY()

public:
	ABaseCollectible();

protected:
	virtual void BeginPlay() override;

	// Override des fonctions d'overlap pour collection automatique
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult) override;

	// Override de OnInteract pour la logique de collectible
	virtual void OnInteract_Implementation(AActor* InstigatorActor) override;

	// Implémentation de l'interface CollectibleInterface
	virtual void OnCollected_Implementation(AActor* Collector) override;

	// ===== PARAMÈTRES EXPOSÉS AU BLUEPRINT =====

	// Type de collectible (Coin, Gem, Health, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Settings", meta = (ExposeOnSpawn = "true"))
	FName CollectibleType = "DefaultCollectible";

	// Quantité donnée lors du ramassage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Settings", meta = (ClampMin = "1"))
	int32 CollectibleAmount = 1;

	// Son au ramassage (override du son d'interaction de base)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Audio")
	USoundBase* CollectionSound;

	// Volume du son de collection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CollectionSoundVolume = 1.0f;

	// Pitch du son de collection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Audio", meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float CollectionSoundPitch = 1.0f;

	// Si true, le collectible se détruit après collection
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Settings")
	bool bDestroyOnCollect = true;

	// Délai avant destruction (pour laisser le temps au son/VFX de jouer)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Settings", meta = (EditCondition = "bDestroyOnCollect", ClampMin = "0.0"))
	float DestroyDelay = 0.1f;

	// Si true, collection automatique au contact (sans appuyer sur E)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible|Settings")
	bool bAutoCollectOnOverlap = true;

	// Event Blueprint appelé après collection (pour VFX, etc.)
	UFUNCTION(BlueprintImplementableEvent, Category = "Collectible|Events")
	void OnCollectedEvent(AActor* Collector);

private:
	void HandleDestruction();
	bool bAlreadyCollected = false;
};