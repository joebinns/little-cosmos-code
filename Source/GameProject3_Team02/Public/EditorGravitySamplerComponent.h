// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravitySamplerComponent.h"
#include "EditorGravitySamplerComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT3_TEAM02_API UEditorGravitySamplerComponent : public UGravitySamplerComponent
{
	GENERATED_BODY()

protected:
	virtual void UpdateGravityFields() override;
	
};
