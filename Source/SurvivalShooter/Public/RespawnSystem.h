#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RespawnSystem.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SURVIVALSHOOTER_API ARespawnSystem : public AActor
{
	GENERATED_BODY()
	
public:
	ARespawnSystem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* VisualMesh;
	
	UPROPERTY(EditAnywhere, Category = "RespawnSystem")
	AActor* RespawnPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "RespawnSystem")
	float Delay = 0.0f;

	UPROPERTY(EditAnywhere, Category = "RespawnSystem")
	FName PlayerTag = FName("PlayerT");
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


private:
	void RespawnActor(AActor* ActorToRespawn);
};