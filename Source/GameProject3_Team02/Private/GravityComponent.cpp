// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityComponent.h"

UGravityComponent::UGravityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	Root = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (!Root)
		UE_LOG(LogTemp, Fatal, TEXT("Gravity Component requires the actor's root is a type of Primitive Component!"));
}

void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	SortGravityFields();

	if (!Enabled)
		return;
	
	Root->AddForce(SampleForce() * DeltaTime * 60.f);
}