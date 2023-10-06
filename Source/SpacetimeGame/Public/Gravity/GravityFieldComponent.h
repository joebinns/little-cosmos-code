// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityFieldComponent.generated.h"

UCLASS( ClassGroup=(GravitationalField), meta=(BlueprintSpawnableComponent) )
class SPACETIMEGAME_API UGravityFieldComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	static UGravityFieldComponent* FocusedGravityField;
	
	static TArray<UGravityFieldComponent*> GravityFields;

	UFUNCTION()
	float GetDistanceToSurface(FVector WorldLocation);

	UFUNCTION()
	FVector GetDirectionToSurface(FVector WorldLocation);

	UFUNCTION()
	FVector GetDisplacementToSurface(FVector WorldLocation);
	
	UFUNCTION()
	FVector GetForce(FVector WorldLocation, float OtherMass);

	UFUNCTION()
	float GetMass();

	UFUNCTION(BlueprintCallable)
	static void SetGravityFields(TArray<AActor*> AllActors); // Do this on level loaded

	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;

protected:
	UPROPERTY()
	float GravitationalConstant = 981000.f;

	UPROPERTY(EditAnywhere, Category="GravitationalField")
	float Mass = 1.f;
	
	virtual float SignedDistance(FVector Location);
	FVector EstimateNormal(FVector Location);
	
};
