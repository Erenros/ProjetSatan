#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CollectibleGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCollectibleCountChanged, FName, CollectibleType, int32, NewCount);

UCLASS()
class BPCPPPROJ_API UCollectibleGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UCollectibleGameInstance();

	// Event broadcast quand un collectible est ramasse
	UPROPERTY(BlueprintAssignable, Category = "Collectibles")
	FOnCollectibleCountChanged OnCollectibleCountChanged;

	// Ajoute un collectible et notifie l'UI
	UFUNCTION(BlueprintCallable, Category = "Collectibles")
	void AddCollectible(FName CollectibleType, int32 Amount = 1);

	// Recup le nombre de collectibles d'un type
	UFUNCTION(BlueprintPure, Category = "Collectibles")
	int32 GetCollectibleCount(FName CollectibleType) const;

	// Recup le total de tous les collectibles
	UFUNCTION(BlueprintPure, Category = "Collectibles")
	int32 GetTotalCollectibles() const;

	// Remet a 0 un type specifique
	UFUNCTION(BlueprintCallable, Category = "Collectibles")
	void ResetCollectible(FName CollectibleType);

	// Remet a 0 tous les collectibles
	UFUNCTION(BlueprintCallable, Category = "Collectibles")
	void ResetAllCollectibles();

	// Recup tous les types de collectibles et leurs counts
	UFUNCTION(BlueprintPure, Category = "Collectibles")
	TMap<FName, int32> GetAllCollectibles() const { return CollectibleCounts; }

protected:
	// Map qui stocke le nombre de chaque type de collectible
	UPROPERTY(BlueprintReadOnly, Category = "Collectibles")
	TMap<FName, int32> CollectibleCounts;
};