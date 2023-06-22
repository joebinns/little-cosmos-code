// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityFieldComponent.h"
#include "SphereGravityFieldComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(GravitationalField), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT3_TEAM02_API USphereGravityFieldComponent : public UGravityFieldComponent
{
	GENERATED_BODY()

public:
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;


protected:
	float GetRadius();
	
	virtual float SignedDistance(FVector Location) override;

#if WITH_EDITORONLY_DATA
	// The sphere component used to visualise the surface of the gravity field
	class UDrawSphereComponent* DrawSphere;
#endif
};
