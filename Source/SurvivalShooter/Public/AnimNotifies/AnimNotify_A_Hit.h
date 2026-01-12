#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_A_Hit.generated.h"

UCLASS(const, DisplayName = "A_Hit")
class SURVIVALSHOOTER_API UAnimNotify_A_Hit : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

