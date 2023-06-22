// Fill out your copyright notice in the Description page of Project Settings.


#include "GravitySamplerComponent.h"
#include "GravityFieldComponent.h"

UGravitySamplerComponent::UGravitySamplerComponent()
{
}

FVector UGravitySamplerComponent::SampleForce()
{
	UpdateGravityFields();
	SortGravityFields();
	FVector TotalForce = FVector::ZeroVector;
	for (int i = 0; i < FGenericPlatformMath::Min(NumEffectiveFields, GravityFields.Num()); i++)
	{
		TotalForce += SampleForceAtIndex(i);
	}
	TotalForce *= ForceFactor;
	return TotalForce;
}

FVector UGravitySamplerComponent::SampleForceDirection()
{
	return SampleForce().GetSafeNormal();
}

float UGravitySamplerComponent::SampleForceMagnitude()
{
	return SampleForce().Length();
}

FVector UGravitySamplerComponent::SampleForceAtIndex(const int Index)
{
	UGravityFieldComponent* GravityField = GravityFields[Index];
	const FVector Force = GravityField->GetForce(GetOwner()->GetActorLocation(), Mass);
	return Force;
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
	
	GravityFields.Sort( 
		[this](UGravityFieldComponent& A, UGravityFieldComponent& B)
		{
			const FVector ActorLocation = GetOwner()->GetActorLocation();
			const float InfluenceA = pow(A.GetDistanceToSurface(ActorLocation), 2.f) / A.GetMass();
			const float InfluenceB = pow(B.GetDistanceToSurface(ActorLocation), 2.f) / B.GetMass();
			return InfluenceA < InfluenceB;
		});

	UGravityFieldComponent* PrimaryGravityField = GravityFields[0];
	if ((PrimaryGravityField != PreviousPrimaryGravityField) & (PreviousPrimaryGravityField != nullptr))
		OnPrimaryGravityFieldChangedDelegate.Broadcast();

	PreviousPrimaryGravityField = PrimaryGravityField;
	
	//UE_LOG(LogTemp, Warning, TEXT("Distance to Surface 1: %f"), GravityFields[0]->GetDistanceToSurface(GetOwner()->GetActorLocation()));
	//UE_LOG(LogTemp, Warning, TEXT("Distance to Surface 2: %f"), GravityFields[1]->GetDistanceToSurface(GetOwner()->GetActorLocation()));

	//UE_LOG(LogTemp, Warning, TEXT("Num Fields: %d"), GravityFields.Num());
}


