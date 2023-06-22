#include "CheckpointHandler.h"

ACheckpointHandler::ACheckpointHandler()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACheckpointHandler::SetNewCheckpoint(FVector NewCheckpoint)
{
	CurrentCheckPoint = NewCheckpoint;
}

FVector ACheckpointHandler::GetCurrentCheckpoint()
{
	return CurrentCheckPoint;
}

