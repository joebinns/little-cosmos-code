// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class SPACETIMEGAME_API FMathsUtilities
{
	public:
		static FRotator CombineRotators(FRotator RotatorA, FRotator RotatorB);

		static FVector MoveTowards(FVector Current, FVector Target, float MaxDeltaDistance);

		static FQuat LookRotation(FVector Forward, FVector Upward);

		static FQuat ShortestRotation(FQuat A, FQuat B);

		static float Dot(FQuat A, FQuat B);
};
