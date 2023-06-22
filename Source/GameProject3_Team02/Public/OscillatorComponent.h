// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "OscillatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT3_TEAM02_API UOscillatorComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	UOscillatorComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	FVector WorldEquilibriumLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	FVector ForceScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	float Stiffness = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	float Damper = 200.f;

	UFUNCTION()
	FVector CalculateRestoringForce(float DeltaTime);
	
	UFUNCTION()
	FVector HookesLaw(FVector Displacement, FVector Velocity);

#pragma region DEBUG

	UPROPERTY(EditAnywhere, Category="Oscillator|Debug")
	float DebugRadius = 62.5f;
	
	UFUNCTION()
	void Debug();

#pragma endregion 

private:
	FVector PreviousDisplacement = FVector::ZeroVector;

	FVector PreviousVelocity = FVector::ZeroVector;
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
