// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Hold/HoldableInterface.h"

#include "Oscillators/OscillatorComponent.h"
#include "Oscillators/TorsionalOscillatorComponent.h"
#include "Gravity/GravityComponent.h"

void IHoldableInterface::Grab()
{
	SetHeld(true);
}

void IHoldableInterface::Release()
{
	SetHeld(false);
}

void IHoldableInterface::SetHeld(bool bHeld)
{
	//InternalOscillator->SetActive(bHeld);
	InternalTorsionalOscillator->SetActive(bHeld);
	InternalGravity->SetActive(!bHeld);
}

UOscillatorComponent* IHoldableInterface::GetOscillator() const
{
	return InternalOscillator;
}

UTorsionalOscillatorComponent* IHoldableInterface::GetTorsionalOscillator() const
{
	return InternalTorsionalOscillator;
}

