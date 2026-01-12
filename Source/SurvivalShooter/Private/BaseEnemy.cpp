#include "BaseEnemy.h"
#include "PatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"

ABaseEnemy::ABaseEnemy()
{
    PrimaryActorTick.bCanEverTick = false;
    
    AttackPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttackPoint"));
    AttackPoint->SetupAttachment(GetMesh());

    PerceptionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PerceptionSphere"));
    PerceptionSphere->SetupAttachment(RootComponent);
    PerceptionSphere->SetSphereRadius(DR);
    PerceptionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void ABaseEnemy::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
    
    if (PerceptionSphere) 
    {
        PerceptionSphere->SetSphereRadius(DR);
    }
    
    if (PatrolPath)
    {
        AAIController* AICon = Cast<AAIController>(GetController());
        if (AICon)
        {
            FVector FirstPoint = GetNextPatrolPoint();
            AICon->MoveToLocation(FirstPoint, AcceptanceRadius);
        }
    }
    GetWorldTimerManager().SetTimer(TimerHandle_CheckPlayer, this, &ABaseEnemy::CheckForPlayer, 0.25f, true);
}

FVector ABaseEnemy::GetNextPatrolPoint()
{
    if (!PatrolPath)
    {
        return GetActorLocation();
    }
    
    int32 NumPoints = PatrolPath->GetSplineComponent()->GetNumberOfSplinePoints();
    if (NumPoints == 0)
    {
        return GetActorLocation();
    }
    
    CurrentIndex = (CurrentIndex + 1) % NumPoints;
    return PatrolPath->GetSplineComponent()->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
}

void ABaseEnemy::CheckForPlayer()
{
    if (bIsDead) return;
    
    if (IsAttacking)
    {
        return;
    }

    if (IsRetreating && RetreatPoint)
    {
        AAIController* AICon = Cast<AAIController>(GetController());
        if (AICon)
        {
            AICon->MoveToActor(RetreatPoint, AcceptanceRadius);
        }

        float DistToRetreat = FVector::Dist(GetActorLocation(), RetreatPoint->GetActorLocation());
        if (DistToRetreat <= AcceptanceRadius + 20.f)
        {
            StopRetreat();
        }

        return;
    }
    
    APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!Player)
    {
        return;
    }
    
    AAIController* AICon = Cast<AAIController>(GetController());
    if (!AICon)
    {
        return;
    }
    
    float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
    
    if (Distance <= RangeToAttack)
    {
        DoAttack();
        return;
    }
    
    if (Distance <= DR)
    {
        if (!bIsChasing)
        {
            bIsChasing = true;
            GetCharacterMovement()->MaxWalkSpeed = 400.f;
        }
        AICon->MoveToActor(Player, 30.f);
    }
    else
    {
        if (bIsChasing)
        {
            bIsChasing = false;
            GetCharacterMovement()->MaxWalkSpeed = 200.f;
            
            if (PatrolPath)
            {
                FVector CurrentPoint = PatrolPath->GetSplineComponent()->GetLocationAtSplinePoint(
                    CurrentIndex, 
                    ESplineCoordinateSpace::World
                );
                AICon->MoveToLocation(CurrentPoint, AcceptanceRadius);
            }
        }
    }
}

void ABaseEnemy::DoAttack()
{
    if (bIsDead)
    {
        return;
    }
    
    if (IsAttacking)
    {
        return;
    }
    
    IsAttacking = true;
    bHasHitThisAttack = false;
    
    AAIController* AICon = Cast<AAIController>(GetController());
    if (AICon)
    {
        AICon->StopMovement();
    }
    
    if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
    {
        if (AttackAnimation)
        {
            AnimInst->Montage_Play(AttackAnimation, 3.0f);
        }
    }
    
    GetWorldTimerManager().SetTimer(TimerHandle_AttackCooldown, this, &ABaseEnemy::ResetAttack, ACooldown, false);
}

void ABaseEnemy::ResetAttack()
{
    IsAttacking = false;
    bHasHitThisAttack = false;
}

void ABaseEnemy::HitPlayer()
{
    if (bHasHitThisAttack)
    {
        return;
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn)
    {
        return;
    }

    FVector Origin = GetActorLocation();
    if (GetMesh() && GetMesh()->DoesSocketExist(TEXT("RightHandSocket")))
    {
        Origin = GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));
    }

    float Distance = FVector::Dist(Origin, PlayerPawn->GetActorLocation());

    if (Distance > RangeToAttack)
    {
        return;
    }

    UGameplayStatics::ApplyDamage(PlayerPawn, DamageAmount, GetController(), this, UDamageType::StaticClass());
    bHasHitThisAttack = true;
}

float ABaseEnemy::TakeDamage(float InDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    if (bIsDead)
    {
        return 0.f;
    }
    
    float DamageApplied = Super::TakeDamage(InDamage, DamageEvent, EventInstigator, DamageCauser);
    if (DamageApplied <= 0.f) DamageApplied = InDamage;

    CurrentHealth -= DamageApplied;
    CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

    // If health drops to zero -> die.
    if (CurrentHealth <= 0.f)
    {
        Die();
        return DamageApplied;
    }

    if (!IsRetreating && MaxHealth > 0.0f)
    {
        float HealthFraction = CurrentHealth / MaxHealth;
        if (HealthFraction <= RetreatHealthThreshold)
        {
            StartRetreat();
        }
    }

    return DamageApplied;
}

void ABaseEnemy::Die()
{
    if (bIsDead)
    {
        return;
    }
    
    bIsDead = true;

    GetWorldTimerManager().ClearTimer(TimerHandle_CheckPlayer);
    GetWorldTimerManager().ClearTimer(TimerHandle_AttackCooldown);

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        AICon->StopMovement();
    }

    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseEnemy::StartRetreat()
{
    if (bIsDead)
    {
        return;
    }
    IsRetreating = true;
    GetCharacterMovement()->MaxWalkSpeed = 300.f;

    if (RetreatPoint)
    {
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            AICon->StopMovement();
            AICon->MoveToActor(RetreatPoint, AcceptanceRadius);
        }
    }
}

void ABaseEnemy::StopRetreat()
{
    IsRetreating = false;
    GetCharacterMovement()->MaxWalkSpeed = 200.f;

    // Resume patrol if available.
    if (PatrolPath)
    {
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            FVector CurrentPoint = PatrolPath->GetSplineComponent()->GetLocationAtSplinePoint(
                FMath::Clamp(CurrentIndex, 0, PatrolPath->GetSplineComponent()->GetNumberOfSplinePoints()-1),
                ESplineCoordinateSpace::World
            );
            AICon->MoveToLocation(CurrentPoint, AcceptanceRadius);
        }
    }
}
