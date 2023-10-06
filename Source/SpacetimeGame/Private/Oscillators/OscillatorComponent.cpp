// Fill out your copyright notice in the Description page of Project Settings.


#include "Oscillators/OscillatorComponent.h"

#include "Utilities/PhysicsUtilities.h"

UOscillatorComponent::UOscillatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UActorComponent::SetAutoActivate(true);
}

void UOscillatorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitialiseSubstepTick(FPhysicsUtilities::GetBodyInstance(GetOwner()->GetRootComponent()));
}

void UOscillatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Debug();
	
	SubstepTick();
}

void UOscillatorComponent::OnSubstepTick(float DeltaTime, FBodyInstance* _)
{
	auto const RestoringForce = CalculateRestoringForce(DeltaTime);
		
	SubstepBodyInstance->AddForce(RestoringForce, false);
}

FVector UOscillatorComponent::CalculateRestoringForce(float DeltaTime)
{
	// Get substep location
	FVector SubstepLocation = SubstepBodyInstance->GetUnrealWorldTransform().GetLocation();
	
	FVector Displacement = SubstepLocation - WorldEquilibriumLocation;
	
	//UE_LOG(LogTemp, Warning, TEXT("Displacement: %s"), *Displacement.ToString());
	
	FVector DeltaDisplacement = Displacement - PreviousDisplacement;
	PreviousDisplacement = Displacement;
	FVector Velocity = DeltaDisplacement / DeltaTime;
	FVector Force = HookesLaw(Displacement, Velocity);
	return Force;
}

FVector UOscillatorComponent::HookesLaw(FVector Displacement, FVector Velocity)
{
	FVector Force = Stiffness * Displacement + Damper * Velocity;
	Force = -Force;
	return Force;
}

void UOscillatorComponent::Debug()
{
#if WITH_EDITORONLY_DATA
	DrawDebugSphere(GetWorld(), WorldEquilibriumLocation, DebugRadius, 8, FColor::Green);
	DrawDebugSphere(GetWorld(), GetOwner()->GetRootComponent()->GetComponentLocation(), DebugRadius * 0.8f, 8, FColor::White);
#endif 
}






