// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Interactions/PlayerHoldComponent.h"

#include "Oscillators/OscillatorComponent.h"
#include "Oscillators/TorsionalOscillatorComponent.h"
#include "Utilities/PhysicsUtilities.h"

UPlayerHoldComponent::UPlayerHoldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerHoldComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitialiseSubstepTick(FPhysicsUtilities::GetBodyInstance(GetOwner()->GetRootComponent()));
}

void UPlayerHoldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Held == nullptr)
	{
		UpdatePotentialHoldable();
		if (PotentialHoldable != nullptr)
		{
			// TODO: Activate outline of holdable
		}
		if (PotentialHoldable != PreviousPotentialHoldable)
		{
			if (PreviousPotentialHoldable != nullptr)
			{
				// TODO: Deactivate outline of previous holdable
			}
		}
	}
	else
	{
		UpdateEquilibriums();
	}
	
	SubstepTick();
}

void UPlayerHoldComponent::OnSubstepTick(float DeltaTime, FBodyInstance* _)
{
	// TODO: Whilst the hold target does not have a body instance, this doesn't make sense to use in the SubstepTick.
	// TODO: Could instead use the player physics actor's body instance from which a virtual hold target is projected.
	//UpdateEquilibriums();
}

void UPlayerHoldComponent::UpdateEquilibriums() // TODO: Should probably support substep tick
{
	if (Held == nullptr)
		return;
	
	Held->GetOscillator()->WorldEquilibriumLocation = HoldTarget->GetComponentLocation();
	Held->GetTorsionalOscillator()->WorldEquilibriumRotation = HoldTarget->GetComponentRotation().Euler();
}

void UPlayerHoldComponent::UpdatePotentialHoldable()
{
	FVector TraceStart = GetOwner()->GetRootComponent()->GetComponentLocation();
	FVector TraceEnd = TraceStart + TraceDirection * MaxReach;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HoldableTraceHit, TraceStart, TraceEnd, HoldableTraceChannel, QueryParams);

#if WITH_EDITORONLY_DATA
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, HoldableTraceHit.bBlockingHit ? FColor::Green : FColor::Red, false, 0.0f,
				  0, 1.0f);
#endif

	auto HitActor = HoldableTraceHit.GetActor();
	
	auto HoldableInterface = Cast<IHoldableInterface>(HitActor);
	
	PotentialHoldable = HoldableInterface;
}

void UPlayerHoldComponent::AttemptGrab()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempting Grab"));
	
	if (PotentialHoldable == nullptr)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Grab Successful"));
	
	PotentialHoldable->Grab();
	Held = PotentialHoldable;
}

void UPlayerHoldComponent::AttemptRelease()
{
	if (Held == nullptr)
		return;
	
	Held->Release();
	Held = nullptr;
}
