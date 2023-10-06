// Fill out your copyright notice in the Description page of Project Settings.


#include "SubstepTick/SubstepTickInterface.h"

void ISubstepTickInterface::InitialiseSubstepTick(FBodyInstance* NewSubstepBodyInstance)
{
	SubstepBodyInstance = NewSubstepBodyInstance;
	
	OnCalculateCustomPhysics.BindRaw(this, &ISubstepTickInterface::OnSubstepTick);
}

void ISubstepTickInterface::SubstepTick()
{
	if (!OnCalculateCustomPhysics.IsBound())
		UE_LOG(LogTemp, Fatal, TEXT("Initialise substep tick before calling it."));
	
	if (!SubstepBodyInstance)
	{
		OnCalculateCustomPhysics.Execute(0.f, nullptr);
	}
	else
	{
		SubstepBodyInstance->AddCustomPhysics(OnCalculateCustomPhysics);
	}
}

void ISubstepTickInterface::OnSubstepTick(float DeltaTime, FBodyInstance* _)
{
	// Implement per instance
}

