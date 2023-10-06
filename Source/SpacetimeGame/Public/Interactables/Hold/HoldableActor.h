// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HoldableInterface.h"
#include "HoldableActor.generated.h"

class UOscillatorComponent;
class UTorsionalOscillatorComponent;
class UGravityComponent;

UCLASS()
class SPACETIMEGAME_API AHoldableActor : public AActor, public IHoldableInterface
{
	GENERATED_BODY()

public:
	AHoldableActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UOscillatorComponent* Oscillator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTorsionalOscillatorComponent* TorsionalOscillator = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGravityComponent* Gravity = nullptr;
	
	virtual void BeginPlay() override;

};
