#include "LevelCompleteActor.h"

#include "PlayerPhysicsActor.h"
#include "Kismet/GameplayStatics.h"

ALevelCompleteActor::ALevelCompleteActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ALevelCompleteActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerPhysicsActor::StaticClass()))
	{
		InGameUI->InGameUiInstance->CallEndLevel();
	}
}

void ALevelCompleteActor::BeginPlay()
{
	Super::BeginPlay();
	InGameUI = Cast<AInGameUIHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), AInGameUIHandler::StaticClass()));
}

void ALevelCompleteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

