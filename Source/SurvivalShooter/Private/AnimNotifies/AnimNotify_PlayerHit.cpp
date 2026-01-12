#include "AnimNotifies/AnimNotify_PlayerHit.h"
#include "ShooterCharacter.h"

void UAnimNotify_PlayerHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	AShooterCharacter* Player = Cast<AShooterCharacter>(Owner);
	if (!Player)
	{
		AActor* Higher = Owner->GetOwner();
		if (Higher)
		{
			Player = Cast<AShooterCharacter>(Higher);
		}
	}

	if (Player)
	{
		Player->DealDamageToEnemy();
	}
}