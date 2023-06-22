// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "PlayerPhysicsActor.h"
#include "Kismet/GameplayStatics.h"

APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APowerUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult)
{
	if(PowerUpHandler && OtherActor->IsA(APlayerPhysicsActor::StaticClass()))
	{
		PowerUpHandler->CollectPowerUp();
	}
	Destroy();
}

void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	PowerUpHandler = Cast<APowerUpHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), APowerUpHandler::StaticClass()));
}

void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

