#include "EndSystem.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AEndSystem::AEndSystem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	BoxComp->SetCollisionProfileName(TEXT("Trigger"));
	BoxComp->SetGenerateOverlapEvents(true);
}

void AEndSystem::BeginPlay()
{
	Super::BeginPlay();
}

void AEndSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEndSystem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!OtherActor)
	{
		return;
	}

	if (!OtherActor->ActorHasTag(PlayerTag))
	{
		return;
	}

	if (!LevelToOpen.IsNone())
	{
		UGameplayStatics::OpenLevel(this, LevelToOpen);
	}
}
