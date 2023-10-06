// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubstepTick/SubstepTickInterface.h"
#include "Components/ActorComponent.h"
#include "OscillatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACETIMEGAME_API UOscillatorComponent : public UActorComponent, public ISubstepTickInterface
{
	GENERATED_BODY()

public:	
	UOscillatorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	FVector WorldEquilibriumLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	FVector ForceScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	float Stiffness = 6000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator")
	float Damper = 2000.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnSubstepTick(float DeltaTime, FBodyInstance* _) override;
	
	FVector CalculateRestoringForce(float DeltaTime);
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
		
};
