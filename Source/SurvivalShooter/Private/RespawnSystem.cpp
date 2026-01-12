#include "RespawnSystem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "ShooterCharacter.h"

ARespawnSystem::ARespawnSystem()
{
	PrimaryActorTick.bCanEverTick = false;

	// BOXCOMPONENT for the Trigger.
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->SetGenerateOverlapEvents(true);


	// Mesh VISUALIZATION.
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(RootComponent);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ARespawnSystem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!OtherActor || OtherActor == this)
	{
		return;
	}
	if (!PlayerTag.IsNone() && !OtherActor->ActorHasTag(PlayerTag))
	{
		return;
	}
	if (Delay > 0.0f)
	{
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("RespawnActor"), OtherActor);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, Delay, false);
		return;
	}
	RespawnActor(OtherActor);
}

void ARespawnSystem::RespawnActor(AActor* ActorToRespawn)
{
	if (!ActorToRespawn)
	{
		return;
	}

	AActor* UseRespawn = RespawnPoint;
	if (!UseRespawn)
	{
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Found);
		if (Found.Num() > 0)
		{
			UseRespawn = Found[0];
		}
	}
	if (!UseRespawn)
	{
		return;
	}

	const FVector TargetLoc = UseRespawn->GetActorLocation();
	const FRotator TargetRot = UseRespawn->GetActorRotation();

	ActorToRespawn->SetActorLocationAndRotation(TargetLoc, TargetRot, false, nullptr, ETeleportType::TeleportPhysics);

	if (AShooterCharacter* Player = Cast<AShooterCharacter>(ActorToRespawn))
	{
		Player->RespawnPlayer();
	}

	UPrimitiveComponent* Prim = ActorToRespawn->FindComponentByClass<UPrimitiveComponent>();
	if (Prim)
	{
		Prim->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Prim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}

	APawn* Pawn = Cast<APawn>(ActorToRespawn);
	if (Pawn)
	{
		if (UPawnMovementComponent* MoveComp = Pawn->FindComponentByClass<UPawnMovementComponent>())
		{
			MoveComp->StopMovementImmediately();
		}
		if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(Pawn->GetRootComponent()))
		{
			RootPrim->SetPhysicsLinearVelocity(FVector::ZeroVector);
			RootPrim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);


		}
	}
}


