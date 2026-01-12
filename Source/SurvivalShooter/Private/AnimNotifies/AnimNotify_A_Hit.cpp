#include "AnimNotifies/AnimNotify_A_Hit.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseEnemy.h"

void UAnimNotify_A_Hit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
	{
		return;
	}
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(Owner);
	if (!Enemy)
	{
		AActor* Higher = Owner->GetOwner();
		if (Higher)
		{
			Enemy = Cast<ABaseEnemy>(Higher);
		}
	}
	if (!Enemy) {
		return; }
	
	Enemy->HitPlayer();
}
