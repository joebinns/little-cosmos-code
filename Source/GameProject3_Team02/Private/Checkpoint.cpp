#include "Checkpoint.h"
#include "PlayerPhysicsActor.h"
#include "Kismet/GameplayStatics.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerPhysicsActor::StaticClass()))
	{
		if (!bCheckPointActivated)
		{
			CheckpointHandler->SetNewCheckpoint(this->GetActorLocation());
			bCheckPointActivated = true;
		}
	}
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	CheckpointHandler = Cast<ACheckpointHandler>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ACheckpointHandler::StaticClass()));
}
