#include "PowerUpHandler.h"
#include "Kismet/GameplayStatics.h"

APowerUpHandler::APowerUpHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APowerUpHandler::BeginPlay()
{
	Super::BeginPlay();
	InGameUIHandler = Cast<AInGameUIHandler>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AInGameUIHandler::StaticClass()));
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALaunchPad::StaticClass(), LaunchPads);
}


void APowerUpHandler::CollectPowerUp()
{
	CollectedPowerUps++;
	if (CollectedPowerUps != PowerUpsRequired)
	{
		InGameUIHandler->InGameUiInstance->IncreaseProgress();
	}
	else
	{
		PlayerActor = Cast<APlayerPhysicsActor>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPhysicsActor::StaticClass()));
		FVector ActorLocation = PlayerActor->GetActorLocation();
		float ClosestDistance = FLT_MAX;
		ClosestLaunchPad = nullptr;
		InGameUIHandler->InGameUiInstance->IncreaseProgress();
		InGameUIHandler->InGameUiInstance->Objective->SetVisibility(ESlateVisibility::Visible);
		for (const auto& Element : LaunchPads)
		{
			ALaunchPad* LaunchPad = Cast<ALaunchPad>(Element);
			LaunchPad->bIsActivated = true;
			float Distance = FVector::DistSquared(ActorLocation, LaunchPad->GetActorLocation());

			if(Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestLaunchPad = LaunchPad;
			}
		}
		if(ClosestLaunchPad)
		{
			ClosestLaunchPad->MegaPadMarker->SetVisibleFlag(true);
		}
	}
}

void APowerUpHandler::ResetObjective()
{
	for (const auto& Element : LaunchPads)
	{
		ALaunchPad* LaunchPad = Cast<ALaunchPad>(Element);
		LaunchPad->bIsActivated = false;
	}
	CollectedPowerUps = 0;
}

void APowerUpHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
