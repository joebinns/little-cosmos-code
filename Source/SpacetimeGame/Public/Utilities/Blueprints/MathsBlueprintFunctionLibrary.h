// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathsBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SPACETIMEGAME_API UMathsBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category="Math")
	static FQuat FindBetweenNormals(FVector Normal1, FVector Normal2);
};
