// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubstepTick/SubstepTickInterface.h"
#include "GravitySamplerComponent.generated.h"

class UGravityFieldComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrimaryGravityFieldChanged, UGravityFieldComponent*, NewPrimaryGravityField);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACETIMEGAME_API UGravitySamplerComponent : public UActorComponent, public ISubstepTickInterface
{
	GENERATED_BODY()

public:	
	UGravitySamplerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category="Gravity")
	float Mass = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	float ForceFactor = 1.f;

	virtual void BeginPlay() override;

	FVector GetForce();
	FVector GetForceDirection();
	float GetForceMagnitude();

	UPROPERTY(BlueprintAssignable)
	FOnPrimaryGravityFieldChanged OnPrimaryGravityFieldChangedDelegate;

protected:
	TArray<UGravityFieldComponent*> GravityFields;

	UPROPERTY(EditAnywhere, Category="Gravity")
	int NumEffectiveFields = 1;
	
	FVector SampleForceAtIndex(const int Index);

	UGravityFieldComponent* GetPrimaryGravityField();
	
	void SortGravityFields();

	virtual void UpdateGravityFields();

	virtual void OnSubstepTick(float DeltaTime, FBodyInstance* _) override;
	
	virtual void UpdateForce();
	virtual void UpdateTimeDilationFactor();
	
	FVector Force;
	FVector ForceDirection;
	float ForceMagnitude;
	
	FVector SampleForce();
	float SampleTimeDilationFactor();

private:
	UGravityFieldComponent* PreviousPrimaryGravityField;
};
