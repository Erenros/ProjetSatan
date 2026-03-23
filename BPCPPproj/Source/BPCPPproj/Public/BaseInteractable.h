#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "BaseInteractable.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class BPCPPPROJ_API ABaseInteractable : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	ABaseInteractable();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* InteractionZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	// ===== AUDIO SETTINGS - EXPOSÉS AU BLUEPRINT =====

	// Son joué lors de l'interaction - EDITABLE DANS LE BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Audio")
	USoundBase* InteractionSound;

	// Volume du son d'interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Audio", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float InteractionSoundVolume = 1.0f;

	// Pitch du son d'interaction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Audio", meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float InteractionSoundPitch = 1.0f;

	// ===== INTERACTION SETTINGS =====

	// Nom affiché dans l'UI (ex: "Press F to respect")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
	FText InteractionPromptText = FText::FromString("Interact");

	// Si true, on peut interagir plusieurs fois
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
	bool bCanInteractMultipleTimes = true;

	// CD entre interactions (en secondes)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings", meta = (ClampMin = "0.0"))
	float InteractionCooldown = 0.0f;

	// Active l'interaction automatique a la collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Settings")
	bool bAutoInteractOnOverlap = false;

	// Implémentation de l'interface
	virtual void OnInteract_Implementation(AActor* InstigatorActor) override;

	// Event BP
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Events")
	void OnInteractedEvent(AActor* InteractingActor);

	// Gestion des overlaps (virtual pour override dans les classes enfants)
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Acteur actuellement dans la zone d'interaction
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentOverlappingActor;

private:
	float LastInteractionTime = -999.0f;

	bool CanInteract() const;
};