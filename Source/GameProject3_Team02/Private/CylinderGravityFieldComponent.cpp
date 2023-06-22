// Fill out your copyright notice in the Description page of Project Settings.


#include "CylinderGravityFieldComponent.h"
#include "Components/CapsuleComponent.h"

float UCylinderGravityFieldComponent::SignedDistance(FVector Location)
{
	Location = FRotator(0.f, 0.f, -90.f).RotateVector(Location); // Align with Unreal's primitive cylinder
	FVector2D D = FVector2D(FVector2D(Location.X, Location.Z).Length() - GetRadius() + CornerRadius, FMath::Abs(Location.Y) - 0.5f * GetHeight());
	return FMath::Min(FMath::Max(D.X, D.Y), 0.f) + FVector2D::Max(D, FVector2D::ZeroVector).Length() - CornerRadius;
}

float UCylinderGravityFieldComponent::GetHeight()
{
	return this->GetComponentScale().Z * 100.f;
}

float UCylinderGravityFieldComponent::GetRadius()
{
	return FMath::Min(this->GetComponentScale().X, this->GetComponentScale().Y) * 50.f;
}

#pragma region VISUALISATION
void UCylinderGravityFieldComponent::OnRegister()
{
#if WITH_EDITORONLY_DATA
	if (AActor* MyOwner = GetOwner())
	{
		if (DrawCylinder == nullptr)
		{
			DrawCylinder = NewObject<UCapsuleComponent>(MyOwner, NAME_None);
			DrawCylinder->SetupAttachment(this);
			DrawCylinder->SetIsVisualizationComponent(true);
			DrawCylinder->SetCapsuleSize(GetRadius(), 0.5f * GetHeight());
		}
	}
#endif

	Super::OnRegister();
}

void UCylinderGravityFieldComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
#if WITH_EDITORONLY_DATA
	if (DrawCylinder != nullptr)
	{
		DrawCylinder->DestroyComponent();
		DrawCylinder = nullptr;
	}
#endif 

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
#pragma endregion