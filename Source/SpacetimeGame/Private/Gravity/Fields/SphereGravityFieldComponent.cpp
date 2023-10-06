// Fill out your copyright notice in the Description page of Project Settings.


#include "Gravity/Fields/SphereGravityFieldComponent.h"
#include "Components/DrawSphereComponent.h"

float USphereGravityFieldComponent::SignedDistance(FVector Location)
{
	return Location.Length() - GetRadius();
}

float USphereGravityFieldComponent::GetRadius()
{
	return this->GetComponentScale().X * 50.f;
}

#pragma region VISUALISATION
void USphereGravityFieldComponent::OnRegister()
{
#if WITH_EDITORONLY_DATA
	if (AActor* MyOwner = GetOwner())
	{
		if (DrawSphere == nullptr)
		{
			DrawSphere = NewObject<UDrawSphereComponent>(MyOwner, NAME_None);
			DrawSphere->SetupAttachment(this);
			DrawSphere->SetIsVisualizationComponent(true);
			DrawSphere->SetSphereRadius(GetRadius());
		}
	}
#endif

	Super::OnRegister();
}

void USphereGravityFieldComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
#if WITH_EDITORONLY_DATA
	if (DrawSphere != nullptr)
	{
		DrawSphere->DestroyComponent();
		DrawSphere = nullptr;
	}
#endif 

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
#pragma endregion
