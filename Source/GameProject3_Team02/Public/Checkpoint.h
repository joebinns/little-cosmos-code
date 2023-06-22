#pragma once

#include "CoreMinimal.h"
#include "CheckpointHandler.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACheckpoint();

#pragma region REFS
	ACheckpointHandler* CheckpointHandler;
#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult);
#pragma endregion
	
	bool bCheckPointActivated = false;

protected:
	virtual void BeginPlay() override;
};
