// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GP3_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT3_TEAM02_API UGP3_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNarrativeModeEnabled = false;
};
