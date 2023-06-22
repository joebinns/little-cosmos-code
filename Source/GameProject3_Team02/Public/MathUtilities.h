// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"

class GAMEPROJECT3_TEAM02_API FMathUtilities
{
	public:
		static FRotator CombineRotators(FRotator RotatorA, FRotator RotatorB);

		static FVector MoveTowards(FVector Current, FVector Target, float MaxDeltaDistance);

		static FQuat LookRotation(FVector Forward, FVector Upward);

		static FQuat ShortestRotation(FQuat A, FQuat B);

		static float Dot(FQuat A, FQuat B);
};
