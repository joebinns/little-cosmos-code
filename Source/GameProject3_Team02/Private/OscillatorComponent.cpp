// Fill out your copyright notice in the Description page of Project Settings.


#include "OscillatorComponent.h"

UOscillatorComponent::UOscillatorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UOscillatorComponent::BeginPlay()
{
	Super::BeginPlay();

	SetSimulatePhysics(true);
}

void UOscillatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector RestoringForce = CalculateRestoringForce(DeltaTime);
	AddForce(RestoringForce);

	Debug();
}

FVector UOscillatorComponent::CalculateRestoringForce(float DeltaTime)
{
	FVector Displacement = GetComponentLocation() - WorldEquilibriumLocation;
	
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
	DrawDebugSphere(GetWorld(), GetComponentLocation(), DebugRadius * 0.8f, 8, FColor::White);
#endif 
}






