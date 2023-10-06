// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Hold/HoldableActor.h"

#include "Oscillators/OscillatorComponent.h"
#include "Oscillators/TorsionalOscillatorComponent.h"
#include "Gravity/GravityComponent.h"

AHoldableActor::AHoldableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Oscillator = CreateDefaultSubobject<UOscillatorComponent>(TEXT("Oscillator"));
	TorsionalOscillator = CreateDefaultSubobject<UTorsionalOscillatorComponent>(TEXT("TorsionalOscillator"));
	Gravity = CreateDefaultSubobject<UGravityComponent>(TEXT("Gravity"));

	// Update holdable interface references (as UPROPERTY is used)
	InternalOscillator = Oscillator;
	InternalTorsionalOscillator = TorsionalOscillator;
	InternalGravity = Gravity;

	// Declare root
	//RootComponent = ...
}

void AHoldableActor::BeginPlay()
{
	Super::BeginPlay();

	Oscillator->Deactivate();
	TorsionalOscillator->Deactivate();
}