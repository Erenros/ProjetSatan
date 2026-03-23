#include "CollectibleGameInstance.h"

UCollectibleGameInstance::UCollectibleGameInstance()
{
}

void UCollectibleGameInstance::AddCollectible(FName CollectibleType, int32 Amount)
{
	if (Amount <= 0) return;

	// Ajoute ou met a jour le count
	int32& CurrentCount = CollectibleCounts.FindOrAdd(CollectibleType);
	CurrentCount += Amount;

	// Broadcast l'event pour l'UI
	OnCollectibleCountChanged.Broadcast(CollectibleType, CurrentCount);

	UE_LOG(LogTemp, Log, TEXT("Collectible added: %s | New count: %d"), *CollectibleType.ToString(), CurrentCount);
}

int32 UCollectibleGameInstance::GetCollectibleCount(FName CollectibleType) const
{
	const int32* Count = CollectibleCounts.Find(CollectibleType);
	return Count ? *Count : 0;
}

int32 UCollectibleGameInstance::GetTotalCollectibles() const
{
	int32 Total = 0;
	for (const auto& Pair : CollectibleCounts)
	{
		Total += Pair.Value;
	}
	return Total;
}

void UCollectibleGameInstance::ResetCollectible(FName CollectibleType)
{
	if (CollectibleCounts.Contains(CollectibleType))
	{
		CollectibleCounts[CollectibleType] = 0;
		OnCollectibleCountChanged.Broadcast(CollectibleType, 0);
	}
}

void UCollectibleGameInstance::ResetAllCollectibles()
{
	for (auto& Pair : CollectibleCounts)
	{
		Pair.Value = 0;
		OnCollectibleCountChanged.Broadcast(Pair.Key, 0);
	}
}