#include "Deathzone.h"
#include "PlayerPhysicsActor.h"
#include "Kismet/GameplayStatics.h"

ADeathzone::ADeathzone()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADeathzone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->IsA(APlayerPhysicsActor::StaticClass()))
	{
		OtherActor->SetActorLocation(CheckpointHandler->GetCurrentCheckpoint());
	}
}

void ADeathzone::BeginPlay()
{
	Super::BeginPlay();
	CheckpointHandler = Cast<ACheckpointHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), ACheckpointHandler::StaticClass()));
}

