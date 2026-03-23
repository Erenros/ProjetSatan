#include "BaseInteractable.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AudioManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ABaseInteractable::ABaseInteractable()
{
	PrimaryActorTick.bCanEverTick = false;

	// Création du Mesh (racine)
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Création de la zone de détection
	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	InteractionZone->SetupAttachment(Mesh);
	InteractionZone->SetBoxExtent(FVector(100.f, 100.f, 100.f));

	// Configuration collision pour détecter
	InteractionZone->SetCollisionProfileName(TEXT("Trigger"));
	InteractionZone->SetGenerateOverlapEvents(true);

	CurrentOverlappingActor = nullptr;
}

void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();

	// Bind des événements d'overlap
	InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractable::OnOverlapBegin);
	InteractionZone->OnComponentEndOverlap.AddDynamic(this, &ABaseInteractable::OnOverlapEnd);
}

void ABaseInteractable::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Vérifier si c'est le joueur (Character)
	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		CurrentOverlappingActor = OtherActor;
		UE_LOG(LogTemp, Warning, TEXT("[BaseInteractable] Player entered zone: %s"), *GetName());

		// Si auto-interaction activée, déclencher directement
		if (bAutoInteractOnOverlap)
		{
			Execute_OnInteract(this, OtherActor);
		}
	}
}

void ABaseInteractable::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentOverlappingActor)
	{
		CurrentOverlappingActor = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("[BaseInteractable] Player left zone: %s"), *GetName());
	}
}

void ABaseInteractable::OnInteract_Implementation(AActor* InstigatorActor)
{
	// Vérification du cooldown et de la possibilité d'interagir
	if (!CanInteract())
	{
		UE_LOG(LogTemp, Warning, TEXT("[BaseInteractable] Cannot interact - cooldown or already used"));
		return;
	}

	// Jouer le son d'interaction
	if (InteractionSound)
	{
		if (UAudioManagerSubsystem* AudioSub = GetWorld()->GetSubsystem<UAudioManagerSubsystem>())
		{
			AudioSub->PlaySoundAtLocation(InteractionSound, GetActorLocation(), InteractionSoundVolume, InteractionSoundPitch);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[BaseInteractable] Interacted with %s by %s"), *GetName(), *InstigatorActor->GetName());

	// Mettre à jour le temps de dernière interaction
	LastInteractionTime = GetWorld()->GetTimeSeconds();

	// Appeler l'event Blueprint pour logique custom
	OnInteractedEvent(InstigatorActor);
}

bool ABaseInteractable::CanInteract() const
{
	// Si l'objet ne peut être interagi qu'une fois et a déjà été interagi
	if (!bCanInteractMultipleTimes && LastInteractionTime > -999.0f)
	{
		return false;
	}

	// Vérifier le cooldown
	if (InteractionCooldown > 0.0f)
	{
		float TimeSinceLastInteraction = GetWorld()->GetTimeSeconds() - LastInteractionTime;
		if (TimeSinceLastInteraction < InteractionCooldown)
		{
			return false;
		}
	}

	return true;
}