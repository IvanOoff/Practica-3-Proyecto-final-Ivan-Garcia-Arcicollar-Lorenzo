#include "PatrolPath.h"
#include "Components/SplineComponent.h"

APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolSpline"));
	SplineComponent->SetMobility(EComponentMobility::Movable);
	SplineComponent->bEditableWhenInherited = true;
	RootComponent = SplineComponent;
}
