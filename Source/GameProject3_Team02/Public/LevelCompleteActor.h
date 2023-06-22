// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameUIHandler.h"
#include "GameFramework/Actor.h"
#include "LevelCompleteActor.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API ALevelCompleteActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelCompleteActor();

	AInGameUIHandler* InGameUI;
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
					int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
