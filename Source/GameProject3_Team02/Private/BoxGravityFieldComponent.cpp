// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGravityFieldComponent.h"
#include "Components/BoxComponent.h"

float UBoxGravityFieldComponent::SignedDistance(FVector Location)
{
	FVector Q = Location.GetAbs() - GetExtents();
	return Q.ComponentMax(FVector::ZeroVector).Length() + FMath::Min(FMath::Max(Q.X, FMath::Max(Q.Y, Q.Z)), 0.f)
		- CornerRadius * GetExtents().GetMin();
}

FVector UBoxGravityFieldComponent::GetExtents()
{
	return this->GetComponentScale() * 50.f;
}

#pragma region VISUALISATION
void UBoxGravityFieldComponent::OnRegister()
{
#if WITH_EDITORONLY_DATA
	if (AActor* MyOwner = GetOwner())
	{
		if (DrawBox == nullptr)
		{
			DrawBox = NewObject<UBoxComponent>(MyOwner, NAME_None);
			DrawBox->SetupAttachment(this);
			DrawBox->SetIsVisualizationComponent(true);
			DrawBox->SetBoxExtent(GetExtents());
		}
	}
#endif

	Super::OnRegister();
}

void UBoxGravityFieldComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
#if WITH_EDITORONLY_DATA
	if (DrawBox != nullptr)
	{
		DrawBox->DestroyComponent();
		DrawBox = nullptr;
	}
#endif 

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
#pragma endregion

