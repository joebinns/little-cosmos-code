// Fill out your copyright notice in the Description page of Project Settings.


#include "MathBlueprintFunctionLibrary.h"

FQuat UMathBlueprintFunctionLibrary::FindBetweenNormals(FVector Normal1, FVector Normal2)
{
	return FQuat::FindBetweenNormals(Normal1, Normal2);
}