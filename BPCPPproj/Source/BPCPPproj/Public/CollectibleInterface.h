#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CollectibleInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCollectibleInterface : public UInterface
{
	GENERATED_BODY()
};

class BPCPPPROJ_API ICollectibleInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnCollected(AActor* Collector);
};