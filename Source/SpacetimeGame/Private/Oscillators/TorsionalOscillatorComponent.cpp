// Fill out your copyright notice in the Description page of Project Settings.


#include "Oscillators/TorsionalOscillatorComponent.h"

#include "Utilities/MathsUtilities.h"
#include "Utilities/PhysicsUtilities.h"

UTorsionalOscillatorComponent::UTorsionalOscillatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UActorComponent::SetAutoActivate(true);
}

void UTorsionalOscillatorComponent::BeginPlay()
{
	Super::BeginPlay();

	InitialiseSubstepTick(FPhysicsUtilities::GetBodyInstance(GetOwner()->GetRootComponent()));
}

void UTorsionalOscillatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Debug();

	SubstepTick();
}

void UTorsionalOscillatorComponent::OnSubstepTick(float DeltaTime, FBodyInstance* _)
{
	auto const RestoringTorque = CalculateRestoringTorque(DeltaTime);

	SubstepBodyInstance->AddTorqueInRadians(RestoringTorque, false);
}

FVector UTorsionalOscillatorComponent::CalculateRestoringTorque(float DeltaTime)
{
	const auto SubstepRotation = SubstepBodyInstance->GetUnrealWorldTransform().GetRotation();
	const auto SubstepAngularVelocity = SubstepBodyInstance->GetUnrealWorldAngularVelocityInRadians();
	const auto EquilibriumRotation = FQuat::MakeFromEuler(WorldEquilibriumRotation);

	auto DeltaRotation = FMathsUtilities::ShortestRotation(SubstepRotation, EquilibriumRotation);
	DeltaRotation.Normalize();
	FVector Axis;
	float Angle;
	DeltaRotation.ToAxisAndAngle(Axis, Angle);
	FVector AngularDisplacement = FMath::DegreesToRadians(Angle) * Axis;

	//UE_LOG(LogTemp, Warning, TEXT("Displacement: %s"), *Displacement.ToString());

	const auto Torque = AngularHookesLaw(AngularDisplacement, SubstepAngularVelocity);
	return Torque;
}

FVector UTorsionalOscillatorComponent::AngularHookesLaw(FVector AngularDisplacement, FVector AngularVelocity)
{
	FVector Torque = FMath::DegreesToRadians(Stiffness * AngularDisplacement + Damper * AngularVelocity);
	Torque = -Torque;
	return Torque;
}

void UTorsionalOscillatorComponent::Debug()
{
#if WITH_EDITORONLY_DATA
	DrawDebugSphere(GetWorld(), WorldPivotLocation, DebugRadius, 8, FColor::Green);
#endif
}
