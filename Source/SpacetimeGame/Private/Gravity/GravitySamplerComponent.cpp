// Fill out your copyright notice in the Description page of Project Settings.


#include "Gravity/GravitySamplerComponent.h"
#include "Gravity/GravityFieldComponent.h"
#include "Utilities/PhysicsUtilities.h"

UGravitySamplerComponent::UGravitySamplerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravitySamplerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitialiseSubstepTick(FPhysicsUtilities::GetBodyInstance(GetOwner()->GetRootComponent()));
}

void UGravitySamplerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SubstepTick();
}

void UGravitySamplerComponent::OnSubstepTick(float DeltaTime, FBodyInstance* _)
{
	UpdateGravityFields();
	SortGravityFields();
	
	if (GravityFields.Num() <= 0)
		return;

	UpdateForce();
	UpdateTimeDilationFactor();
}

void UGravitySamplerComponent::UpdateTimeDilationFactor()
{
	if (UGravityFieldComponent::FocusedGravityField == nullptr)
		return;
	
	const auto TimeDilationFactor = SampleTimeDilationFactor();
	GetOwner()->CustomTimeDilation = TimeDilationFactor;
	
	//UE_LOG(LogTemp, Warning, TEXT("%s Time Dilation Factor: %f"), *GetOwner()->GetActorNameOrLabel(), TimeDilationFactor);
}

void UGravitySamplerComponent::UpdateForce()
{
	Force = SampleForce();
	ForceDirection = Force.GetSafeNormal();
	ForceMagnitude = Force.Length();
}

FVector UGravitySamplerComponent::GetForce()
{
	return Force;
}

FVector UGravitySamplerComponent::GetForceDirection()
{
	return ForceDirection;
}

float UGravitySamplerComponent::GetForceMagnitude()
{
	return ForceMagnitude;
}

FVector UGravitySamplerComponent::SampleForce()
{
	FVector TotalForce = FVector::ZeroVector;
	for (int i = 0; i < FGenericPlatformMath::Min(NumEffectiveFields, GravityFields.Num()); i++)
	{
		TotalForce += SampleForceAtIndex(i);
	}
	TotalForce *= ForceFactor;
	return TotalForce;
}

FVector UGravitySamplerComponent::SampleForceAtIndex(const int Index)
{
	UGravityFieldComponent* GravityField = GravityFields[Index];

	// Get substep location
	FVector SubstepActorLocation = SubstepBodyInstance == nullptr ? GetOwner()->GetActorLocation() : SubstepBodyInstance->GetUnrealWorldTransform().GetLocation();
	
	const FVector ForceAtIndex = GravityField->GetForce(SubstepActorLocation, Mass);
	return ForceAtIndex;
}

void UGravitySamplerComponent::UpdateGravityFields()
{
	// Get the gravitational field components, in case of any updates
	GravityFields = UGravityFieldComponent::GravityFields;
}

// Sort gravitational fields in descending order by their influence: A weighted distance.
void UGravitySamplerComponent::SortGravityFields()
{
	if (GravityFields.Num() <= 0)
		return;

	// Get substep location
	FVector SubstepActorLocation = SubstepBodyInstance == nullptr ? GetOwner()->GetActorLocation() : SubstepBodyInstance->GetUnrealWorldTransform().GetLocation();
	
	GravityFields.Sort( 
		[SubstepActorLocation](UGravityFieldComponent& A, UGravityFieldComponent& B)
		{
			const float InfluenceA = pow(A.GetDistanceToSurface(SubstepActorLocation), 2.f) / A.GetMass();
			const float InfluenceB = pow(B.GetDistanceToSurface(SubstepActorLocation), 2.f) / B.GetMass();
			return InfluenceA < InfluenceB;
		});

	UGravityFieldComponent* PrimaryGravityField = GetPrimaryGravityField();
	if (PrimaryGravityField != PreviousPrimaryGravityField)
		OnPrimaryGravityFieldChangedDelegate.Broadcast(PrimaryGravityField);

	PreviousPrimaryGravityField = PrimaryGravityField;
	
	//UE_LOG(LogTemp, Warning, TEXT("Distance to Surface 1: %f"), GravityFields[0]->GetDistanceToSurface(GetOwner()->GetActorLocation()));
	//UE_LOG(LogTemp, Warning, TEXT("Distance to Surface 2: %f"), GravityFields[1]->GetDistanceToSurface(GetOwner()->GetActorLocation()));

	//UE_LOG(LogTemp, Warning, TEXT("Num Fields: %d"), GravityFields.Num());
}

float UGravitySamplerComponent::SampleTimeDilationFactor()
{
	auto const TimeDilationFactor = UGravityFieldComponent::FocusedGravityField->GetMass() / GetPrimaryGravityField()->GetMass();
	return TimeDilationFactor;
}

UGravityFieldComponent* UGravitySamplerComponent::GetPrimaryGravityField()
{
	return GravityFields.Num() > 0 ? GravityFields[0] : nullptr;
}
