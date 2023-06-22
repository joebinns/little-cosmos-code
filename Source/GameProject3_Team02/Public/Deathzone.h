#pragma once

#include "CoreMinimal.h"
#include "CheckpointHandler.h"
#include "GameFramework/Actor.h"
#include "Deathzone.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API ADeathzone : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeathzone();

#pragma region REFS
	ACheckpointHandler* CheckpointHandler;
#pragma endregion 

#pragma region FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);
#pragma endregion 

protected:
	virtual void BeginPlay() override;
};
