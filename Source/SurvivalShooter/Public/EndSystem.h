#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndSystem.generated.h"

UCLASS()
class SURVIVALSHOOTER_API AEndSystem : public AActor
{
	GENERATED_BODY()
	
public: 	
	AEndSystem();
	virtual void Tick(float DeltaTime) override;


protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UBoxComponent* BoxComp;

	// Tag of the player.
	UPROPERTY(EditAnywhere, Category="Level")
	FName PlayerTag = FName("PlayerT");

	// Level name.
		UPROPERTY(EditAnywhere, Category="Level")
	FName LevelToOpen = FName("FinalMenu");
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:

};
