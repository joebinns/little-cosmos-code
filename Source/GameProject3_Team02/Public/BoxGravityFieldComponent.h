// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityFieldComponent.h"
#include "BoxGravityFieldComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(GravitationalField), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT3_TEAM02_API UBoxGravityFieldComponent : public UGravityFieldComponent
{
	GENERATED_BODY()

public:
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:
	UPROPERTY(EditAnywhere, Category="GravitationalField")
	float CornerRadius = 0.f;
	
	FVector GetExtents();

	virtual float SignedDistance(FVector Location) override;

#if WITH_EDITORONLY_DATA
	// The box component used to visualise the surface of the gravity field
	class UBoxComponent* DrawBox;
#endif
	
};
