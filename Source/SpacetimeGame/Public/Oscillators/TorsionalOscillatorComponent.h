// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubstepTick/SubstepTickInterface.h"
#include "Components/ActorComponent.h"
#include "TorsionalOscillatorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACETIMEGAME_API UTorsionalOscillatorComponent : public UActorComponent, public ISubstepTickInterface
{
	GENERATED_BODY()

public:	
	UTorsionalOscillatorComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator", meta=(ToolTip="The local space rotation about which oscillations are centered, in degrees."))
	FVector WorldEquilibriumRotation = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator", meta=(ToolTip="The center about which rotations should occur."))
	FVector WorldPivotLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator", meta=(ToolTip="The world axes over which the oscillator applies torque. Within range [0, 1]."))
	FVector TorqueScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator", meta=(ToolTip="The greater the stiffness constant, the lesser the amplitude of oscillations."))
	float Stiffness = 80000000000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Oscillator", meta=(ToolTip="The greater the damper constant, the faster that oscillations will disappear."))
	float Damper = 400000000.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnSubstepTick(float DeltaTime, FBodyInstance* _) override;
	
	FVector CalculateRestoringTorque(float DeltaTime);
	FVector AngularHookesLaw(FVector AngularDisplacement, FVector AngularVelocity);

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
