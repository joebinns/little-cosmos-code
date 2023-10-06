// Fill out your copyright notice in the Description page of Project Settings.


#include "Gravity/Editor/GravityOrientedActor.h"
#include "Gravity/Editor/EditorGravitySamplerComponent.h"
#include "Kismet/GameplayStatics.h"

AGravityOrientedActor::AGravityOrientedActor()
{
	// Create components
	EditorGravitySampler = CreateDefaultSubobject<UEditorGravitySamplerComponent>(TEXT("EditorGravitySampler"));
	GravityOriented = CreateDefaultSubobject<USceneComponent>(TEXT("GravityOriented"));

	// Declare root
	RootComponent = GravityOriented;
}

#if WITH_EDITOR
void AGravityOrientedActor::PostEditMove(bool bFinished)
{
	// Reset the rotation when initialised
	if (PreviousGravityDirection == FVector::ZeroVector)
		ResetOrientation();
	else
		Orient();
}
#endif

void AGravityOrientedActor::Orient()
{
	// Rotate to face in the up direction from gravity sampler component
	FVector GravityDirection = EditorGravitySampler->GetForceDirection() * (IsOrientationInverted ? 1.f : -1.f);
	FQuat DeltaCurrentRotation = FQuat::FindBetweenNormals(PreviousGravityDirection, GravityDirection);
	PreviousGravityDirection = GravityDirection;

	// Add world rotation to the root scene component
	GravityOriented->AddWorldRotation(DeltaCurrentRotation);
}

void AGravityOrientedActor::ResetOrientation()
{
	PreviousGravityDirection = FVector::UpVector;
	
	// Rotate to face in the up direction from gravity sampler component
	FVector GravityDirection = EditorGravitySampler->GetForceDirection() * (IsOrientationInverted ? 1.f : -1.f);
	FQuat DeltaCurrentRotation = FQuat::FindBetweenNormals(PreviousGravityDirection, GravityDirection);
	PreviousGravityDirection = GravityDirection;

	// Add world rotation to the root scene component
	GravityOriented->SetWorldRotation(DeltaCurrentRotation);
}

void AGravityOrientedActor::InvertOrientation()
{
	IsOrientationInverted = !IsOrientationInverted;
	Orient();
}

