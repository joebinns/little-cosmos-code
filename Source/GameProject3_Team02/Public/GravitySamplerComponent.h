// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravitySamplerComponent.generated.h"

class UGravityFieldComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrimaryGravityFieldChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT3_TEAM02_API UGravitySamplerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGravitySamplerComponent();

	UPROPERTY(EditAnywhere, Category="Gravity")
	float Mass = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	float ForceFactor = 1.f;
	
	UFUNCTION(BlueprintCallable)
	FVector SampleForce();
	
	UFUNCTION(BlueprintCallable)
	FVector SampleForceDirection();

	UFUNCTION(BlueprintCallable)
	float SampleForceMagnitude();

protected:
	TArray<UGravityFieldComponent*> GravityFields;

	UPROPERTY(EditAnywhere, Category="Gravity")
	int NumEffectiveFields = 1;
	
	FVector SampleForceAtIndex(int Index);
	
	void SortGravityFields();

	virtual void UpdateGravityFields();

	UPROPERTY(BlueprintAssignable)
	FOnPrimaryGravityFieldChanged OnPrimaryGravityFieldChangedDelegate;

private:
	UGravityFieldComponent* PreviousPrimaryGravityField;
};
