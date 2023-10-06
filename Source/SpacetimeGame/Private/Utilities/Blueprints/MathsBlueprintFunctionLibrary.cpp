// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/Blueprints/MathsBlueprintFunctionLibrary.h"

FQuat UMathsBlueprintFunctionLibrary::FindBetweenNormals(FVector Normal1, FVector Normal2)
{
	return FQuat::FindBetweenNormals(Normal1, Normal2);
}