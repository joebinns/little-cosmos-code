// Fill out your copyright notice in the Description page of Project Settings.


#include "Gravity/GravityFieldComponent.h"

#include "Kismet/GameplayStatics.h"

UGravityFieldComponent* UGravityFieldComponent::FocusedGravityField;
TArray<UGravityFieldComponent*> UGravityFieldComponent::GravityFields;

void UGravityFieldComponent::OnRegister()
{
	Super::OnRegister();
	GravityFields.AddUnique(this);
	//UE_LOG(LogTemp, Warning, TEXT("Registered: %d"), GravityFields.Num());
}

void UGravityFieldComponent::OnUnregister()
{
	GravityFields.Remove(this);
	//UE_LOG(LogTemp, Warning, TEXT("Unregistered: %d"), GravityFields.Num());
	Super::OnUnregister();
}

void UGravityFieldComponent::BeginPlay()
{
	Super::BeginPlay();
	//GravityFields.Empty();
	//UE_LOG(LogTemp, Warning, TEXT("Empty: %d"), GravityFields.Num());
}

void UGravityFieldComponent::SetGravityFields(TArray<AActor*> AllActors)
{
	GravityFields.Empty();
	for (AActor* Actor : AllActors)
	{
		TArray<UGravityFieldComponent*> ActorsGravityFields;
		Actor->GetComponents<UGravityFieldComponent>(ActorsGravityFields);
		if (ActorsGravityFields.Num() > 0)
			GravityFields.Append(ActorsGravityFields);
	}
}

FVector UGravityFieldComponent::GetDisplacementToSurface(FVector WorldLocation)
{
	return GetDistanceToSurface(WorldLocation) * GetDirectionToSurface(WorldLocation);
}

float UGravityFieldComponent::GetDistanceToSurface(FVector WorldLocation)
{
	// Transform from world space to local space
	FVector Location = GetComponentTransform().InverseTransformVectorNoScale(WorldLocation);
	FVector FieldOffset = GetComponentTransform().InverseTransformVectorNoScale(GetComponentLocation());

	float Distance = SignedDistance(Location - FieldOffset);
	return Distance;
}

FVector UGravityFieldComponent::GetDirectionToSurface(FVector WorldLocation)
{
	// Transform from world space to local space
	FVector Location = GetComponentTransform().InverseTransformVectorNoScale(WorldLocation);
	FVector FieldOffset = GetComponentTransform().InverseTransformVectorNoScale(GetComponentLocation());
	
	FVector Direction = EstimateNormal(Location - FieldOffset);
	FVector WorldDirection = GetComponentTransform().TransformVectorNoScale(Direction);
	return WorldDirection;
}

FVector UGravityFieldComponent::GetForce(FVector WorldLocation, float OtherMass)
{
	const FVector Force = GetDirectionToSurface(WorldLocation) * GravitationalConstant * Mass * OtherMass;
	return Force;
}

float UGravityFieldComponent::GetMass()
{
	return Mass;
}

FVector UGravityFieldComponent::EstimateNormal(FVector Location)
{
	// Estimate the normal by sampling the signed distance field
	const float EPSILON = 1.f; // TODO: How can I determine what the most appropriate value for this will be?
	FVector Direction = - FVector(
	SignedDistance(Location + EPSILON * FVector::ForwardVector)
	- SignedDistance(Location - EPSILON * FVector::ForwardVector),
	SignedDistance(Location + EPSILON * FVector::RightVector)
	- SignedDistance(Location - EPSILON * FVector::RightVector),
	SignedDistance(Location + EPSILON * FVector::UpVector)
	- SignedDistance(Location - EPSILON * FVector::UpVector)
	);
	return Direction.GetSafeNormal();
}

float UGravityFieldComponent::SignedDistance(FVector Location)
{
	// Implement this within children classes
	return 0.f;
}
