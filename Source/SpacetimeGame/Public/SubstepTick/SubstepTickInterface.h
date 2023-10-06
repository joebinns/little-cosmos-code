// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubstepTickInterface.generated.h"

UINTERFACE()
class SPACETIMEGAME_API USubstepTickInterface : public UInterface
{
	GENERATED_BODY()
};

class SPACETIMEGAME_API ISubstepTickInterface
{
	GENERATED_BODY()
	
protected:
	FBodyInstance* SubstepBodyInstance = nullptr;
	
	void InitialiseSubstepTick(FBodyInstance* NewSubstepBodyInstance);
	void SubstepTick();
	virtual void OnSubstepTick(float DeltaTime, FBodyInstance* _);
	
private:
	DECLARE_DELEGATE_TwoParams(FCalculateCustomPhysics, float, FBodyInstance*);
	FCalculateCustomPhysics OnCalculateCustomPhysics;
	
};
