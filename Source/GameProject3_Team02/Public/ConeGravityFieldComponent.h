// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityFieldComponent.h"
#include "ConeGravityFieldComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(GravitationalField), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT3_TEAM02_API UConeGravityFieldComponent : public UGravityFieldComponent
{
	GENERATED_BODY()

protected:
	float GetHeight();
	float GetAngle();

	virtual float SignedDistance(FVector Location) override;
};
