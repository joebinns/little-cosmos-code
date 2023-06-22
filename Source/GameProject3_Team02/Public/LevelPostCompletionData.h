// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelPostCompletionData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GAMEPROJECT3_TEAM02_API FLevelPostCompletionData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FString LevelRating;


	UPROPERTY()
	FString RankingStrength;
	//yup.
};
