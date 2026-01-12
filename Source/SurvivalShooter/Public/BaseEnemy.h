#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class APatrolPath;
class USphereComponent;
class UAnimMontage;

UCLASS()
class SURVIVALSHOOTER_API ABaseEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    ABaseEnemy();
    FVector GetNextPatrolPoint();

    FORCEINLINE bool IsChasing() const { return bIsChasing; }
    FORCEINLINE float GetAcceptanceRadius() const { return AcceptanceRadius; }

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void HitPlayer();

    virtual float TakeDamage(float InDamage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsDead() const { return bIsDead; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    APatrolPath* PatrolPath;

    UPROPERTY(VisibleAnywhere, Category = "Vision")
    USphereComponent* PerceptionSphere;

    UPROPERTY(EditAnywhere, Category = "Vision")
    float DR = 800.f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float RangeToAttack = 150.f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    UAnimMontage* AttackAnimation;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float ACooldown = 1.1f;

    UPROPERTY(EditAnywhere, Category = "Attack")
    float DamageAmount = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Patrol")
    float AcceptanceRadius = 100.f;

    UPROPERTY(EditAnywhere, Category = "Health")
    float MaxHealth = 3.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    bool bIsDead = false;

    // RETREAT BEHAVIOR.
    UPROPERTY(EditAnywhere, Category = "Retreat")
    AActor* RetreatPoint;

    UPROPERTY(EditAnywhere, Category = "Retreat")
    float RetreatHealthThreshold = 0.4f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Retreat")
    bool IsRetreating = false;

    UFUNCTION(BlueprintCallable, Category = "Retreat")
    void StartRetreat();

    UFUNCTION(BlueprintCallable, Category = "Retreat")
    void StopRetreat();

    void CheckForPlayer();
    void DoAttack();
    void ResetAttack();
    void Die();

private:
    UPROPERTY(VisibleAnywhere, Category = "Attack")
    USceneComponent* AttackPoint;
    
    int32 CurrentIndex = -1;
    bool IsAttacking = false;
    bool bIsChasing = false;
    bool bHasHitThisAttack = false;
    FTimerHandle TimerHandle_CheckPlayer;
    FTimerHandle TimerHandle_AttackCooldown;
};