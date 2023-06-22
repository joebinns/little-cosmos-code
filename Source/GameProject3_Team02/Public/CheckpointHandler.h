#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckpointHandler.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API ACheckpointHandler : public AActor
{
	GENERATED_BODY()

public:
	ACheckpointHandler();

#pragma region CHECKPOINT-HANDLER-STUFF
	FVector CurrentCheckPoint;
	void SetNewCheckpoint(FVector NewCheckpoint);
	FVector GetCurrentCheckpoint();
#pragma endregion
};
