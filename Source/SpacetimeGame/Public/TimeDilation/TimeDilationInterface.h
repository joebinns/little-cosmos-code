// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeDilationInterface.generated.h"

UINTERFACE()
class SPACETIMEGAME_API UTimeDilationInterface : public UInterface
{
	GENERATED_BODY()
};

class SPACETIMEGAME_API ITimeDilationInterface
{
	GENERATED_BODY()
	
public:
	float TimeDilationFactor= 1.f;
	
};
