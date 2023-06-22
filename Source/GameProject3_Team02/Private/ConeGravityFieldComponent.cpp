// Fill out your copyright notice in the Description page of Project Settings.


#include "ConeGravityFieldComponent.h"

float UConeGravityFieldComponent::SignedDistance(FVector Location)
{
	return 0.f;
}

float UConeGravityFieldComponent::GetHeight()
{
	return this->GetComponentScale().Z * 100.f;
}

float UConeGravityFieldComponent::GetAngle()
{
	return 0.f;
}