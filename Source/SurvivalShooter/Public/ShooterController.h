#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SURVIVALSHOOTER_API AShooterController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* MappingContext;
	
};