#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AudioManagerSubsystem.generated.h"

class USoundBase;

UCLASS()
class BPCPPPROJ_API UAudioManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Joue un son 2D
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlaySound2D(USoundBase* Sound, float Volume = 1.0f, float Pitch = 1.0f);

	// Joue un son à un endroit précis
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlaySoundAtLocation(USoundBase* Sound, FVector Location, float Volume = 1.0f, float Pitch = 1.0f);
};