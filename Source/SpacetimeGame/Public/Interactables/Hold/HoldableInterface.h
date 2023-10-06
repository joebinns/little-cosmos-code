// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoldableInterface.generated.h"

class UOscillatorComponent;
class UTorsionalOscillatorComponent;
class UGravityComponent;

UINTERFACE()
class SPACETIMEGAME_API UHoldableInterface : public UInterface
{
	GENERATED_BODY()
};

class SPACETIMEGAME_API IHoldableInterface
{
	GENERATED_BODY()

public:
	void Grab();

	void Release();
	
	UOscillatorComponent* GetOscillator() const;

	UTorsionalOscillatorComponent* GetTorsionalOscillator() const;
	
protected:
	UOscillatorComponent* InternalOscillator = nullptr;
	UTorsionalOscillatorComponent* InternalTorsionalOscillator = nullptr;
	UGravityComponent* InternalGravity = nullptr;
	
	void SetHeld(bool bHeld);
	
};
