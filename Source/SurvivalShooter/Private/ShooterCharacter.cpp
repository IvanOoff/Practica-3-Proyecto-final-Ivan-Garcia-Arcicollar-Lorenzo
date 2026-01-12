#include "EnhancedInputComponent.h"
#include "ShooterCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BaseEnemy.h"
#include "DrawDebugHelpers.h"
#include "InputCoreTypes.h"
#include "GameFramework/CharacterMovementComponent.h"

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("PlayerT"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
	WeaponMesh->SetSimulatePhysics(false);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentPlayerHealth = MaxPlayerHealth;

	if (WeaponBPClass)
	{
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(WeaponBPClass);
		EquipWeaponActor(Spawned); 
		EquippedWeapon = Spawned;
	}

	if (DefaultMappingContext)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}
}

void AShooterCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!DefaultMappingContext)
	{
		return;
	}

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

}

void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input)
	{
		return;
	}

	if (DefaultMappingContext)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->AddMappingContext(DefaultMappingContext, 0);
				}
			}
		}
	}

	// MOVE BIND.
	if (MoveAction)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
	}
	
	// LOOK BIND.
	if (LookAction)
	{
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
	}

	// ATTACK BIND.
	if (AttackAction)
	{
		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Attack);
	}

	// JUMPp BIND.
	if (JumpAction)
	{
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterCharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &AShooterCharacter::StopJumping);
	}
}

void AShooterCharacter::Move(const FInputActionValue& InputActionValue)
{
	FVector2D InputVector = InputActionValue.Get<FVector2D>();
	FRotator YawRotator = FRotator(0, GetControlRotation().Yaw, 0);
	
	FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);


	AddMovementInput(ForwardVector, InputVector.Y);
	AddMovementInput(RightVector, InputVector.X);
}

void AShooterCharacter::Look(const FInputActionValue& InputActionValue)
{
	FVector2D MouseLook = InputActionValue.Get<FVector2D>();

	AddControllerPitchInput(-MouseLook.Y);
	AddControllerYawInput(MouseLook.X);

}

void AShooterCharacter::EquipWeaponActor(AActor* WeaponActor)
{
	WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandSocket"));
}

// Attack method  -> animation montage play.
void AShooterCharacter::Attack()
{
	// Cooldown -> cant attack if on cooldown.
	if (!canAttack)
	{
		return;
	}
	canAttack = false;

	if (UAnimInstance* AnimInst = GetMesh()->GetAnimInstance())
	{
		if (AttackMontage)
		{
			AnimInst->Montage_Play(AttackMontage, 1.0f);
		}
	}

	GetWorldTimerManager().SetTimer(AttackCooldownTimerHandle, this, &AShooterCharacter::ResetAttackCooldown, AttackCooldown, false);
}

void AShooterCharacter::ResetAttackCooldown()
{
	canAttack = true;
}

void AShooterCharacter::Dplayer()
{
	dead = true;
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

}

void AShooterCharacter::RespawnPlayer()
{
	dead = false;
	CurrentPlayerHealth = MaxPlayerHealth;

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		EnableInput(PC);
	}
	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->Velocity = FVector::ZeroVector;
		MoveComp->SetMovementMode(MOVE_Walking);
	}

	if (UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		RootPrim->SetPhysicsLinearVelocity(FVector::ZeroVector);
		RootPrim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (DamageApplied <= 0.f) DamageApplied = DamageAmount;

	CurrentPlayerHealth -= DamageApplied;

	if (CurrentPlayerHealth <= 0.f && !dead)
	{
		CurrentPlayerHealth = 0.f;
		Dplayer();
	}

	return DamageApplied;
}

void AShooterCharacter::DealDamageToEnemy()
{
	if (dead)
	{
		return;
	}

	FVector Start;
	if (GetMesh() && GetMesh()->DoesSocketExist(TEXT("RightHandSocket")))
	{
		Start = GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));
	}
	else
	{
		Start = GetActorLocation() + FVector(0,0,50.f);
	}

	FRotator AimRot = GetControlRotation();
	FVector Forward = AimRot.Vector();

	float UseRange = FMath::Max(AttackRange, 300.f);
	FVector End = Start + (Forward * UseRange);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(60.f);
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, ObjectQueryParams, Sphere, Params);

	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();
		ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitActor);
		if (!Enemy)
		{
			if (HitActor->GetOwner())
			{
				Enemy = Cast<ABaseEnemy>(HitActor->GetOwner());
			}
		}

		if (Enemy)
		{
			UGameplayStatics::ApplyDamage(Enemy, PlayerDamageAmount, GetController(), this, UDamageType::StaticClass());
		}
	}
}
