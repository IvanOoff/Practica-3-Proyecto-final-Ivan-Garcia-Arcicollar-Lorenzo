#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class USplineComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(DisplayName="Patrol Path"))
class SURVIVALSHOOTER_API APatrolPath : public AActor
{
	GENERATED_BODY()

public:
	APatrolPath();

	// Spline.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol", meta=(AllowPrivateAccess="true"))
	USplineComponent* SplineComponent;	
	USplineComponent* GetSplineComponent() const { return SplineComponent; }
};
