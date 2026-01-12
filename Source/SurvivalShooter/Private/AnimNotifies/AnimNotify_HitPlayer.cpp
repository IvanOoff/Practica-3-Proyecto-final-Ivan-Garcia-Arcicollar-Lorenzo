#include "AnimNotifies/AnimNotify_HitPlayer.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_HitPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimNotify_HitPlayer: MeshComp o Owner es null"));
		return;
	}

	ABaseEnemy* Enemy = Cast<ABaseEnemy>(MeshComp->GetOwner());
	if (Enemy)
	{
		Enemy->HitPlayer();
		UE_LOG(LogTemp, Warning, TEXT("AnimNotify_HitPlayer: HitPlayer() llamado en %s"), *Enemy->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimNotify_HitPlayer: Owner no es BaseEnemy"));
	}
}