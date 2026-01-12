#pragma once

#include "InputAction.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "TimerManager.h"
#include "ShooterCharacter.generated.h"

struct FInputActionValue;

class UStaticMeshComponent;
class AActor;

UCLASS()
class SURVIVALSHOOTER_API AShooterCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AShooterCharacter();

    UFUNCTION(BlueprintCallable, Category = "Respawn")
    void RespawnPlayer();

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;

public:     
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetCurrentHealth() const { return CurrentPlayerHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetMaxHealth() const { return MaxPlayerHealth; }

    UFUNCTION(BlueprintPure, Category = "Health")
    bool IsPlayerDead() const { return dead; }

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void DealDamageToEnemy();


private:

    UPROPERTY(EditAnywhere, Category = "Inputs")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Inputs")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Inputs")
    UInputAction* AttackAction;

    UPROPERTY(EditAnywhere, Category = "Inputs")
    UInputAction* JumpAction;
    
    UPROPERTY(EditAnywhere, Category = "Animation")
    class UAnimMontage* AttackMontage;
    
    UPROPERTY(EditAnywhere, Category = "Inputs")
    UInputMappingContext* DefaultMappingContext;

    void Move(const FInputActionValue& InputActionValue);
    void Look(const FInputActionValue& InputActionValue);
    void Attack();
    void ResetAttackCooldown();

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;
    void EquipWeaponActor(AActor* WeaponActor);

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AActor> WeaponBPClass;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    AActor* EquippedWeapon;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackCooldown = 1.0f;
    
    UPROPERTY(EditAnywhere, Category = "Combat")
    bool canAttack = true;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float PlayerDamageAmount = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackRange = 200.f;

    UPROPERTY(EditAnywhere, Category = "Health")
    float MaxPlayerHealth = 4.0f;

    UPROPERTY(EditAnywhere, Category = "Health")
    float CurrentPlayerHealth;

    UPROPERTY(EditAnywhere, Category = "Health")
    bool dead = false;

    void Dplayer();

    FTimerHandle AttackCooldownTimerHandle;
};
