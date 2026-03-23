#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class BPCPPPROJ_API IInteractionInterface
{
	GENERATED_BODY()

public:
	// FQuand le joueur appuie sur E ou marche dessus
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* InstigatorActor);
};