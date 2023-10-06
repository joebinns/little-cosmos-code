// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gravity/GravityFieldComponent.h"
#include "CylinderGravityFieldComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(GravitationalField), meta=(BlueprintSpawnableComponent) )
class SPACETIMEGAME_API UCylinderGravityFieldComponent : public UGravityFieldComponent
{
	GENERATED_BODY()
	
public:
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:
	UPROPERTY(EditAnywhere, Category="GravitationalField")
	float CornerRadius = 0.f;
	
	float GetHeight();
	float GetRadius();

	virtual float SignedDistance(FVector Location) override;

#if WITH_EDITORONLY_DATA
	// The capsule component used to visualise the surface of the gravity field
	class UCapsuleComponent* DrawCylinder;
#endif
	
};
