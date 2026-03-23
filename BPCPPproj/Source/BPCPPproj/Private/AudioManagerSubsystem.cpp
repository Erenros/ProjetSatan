#include "AudioManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UAudioManagerSubsystem::PlaySound2D(USoundBase* Sound, float Volume, float Pitch)
{
	if (!Sound) return;
	UGameplayStatics::PlaySound2D(GetWorld(), Sound, Volume, Pitch);
}

void UAudioManagerSubsystem::PlaySoundAtLocation(USoundBase* Sound, FVector Location, float Volume, float Pitch)
{
	if (!Sound) return;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, Volume, Pitch);
}