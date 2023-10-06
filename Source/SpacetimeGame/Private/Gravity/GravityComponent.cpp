// Fill out your copyright notice in the Description page of Project Settings.


#include "Gravity/GravityComponent.h"
#include "Utilities/PhysicsUtilities.h"

UGravityComponent::UGravityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	UActorComponent::SetAutoActivate(true);
}

void UGravityComponent::BeginPlay()
{
	Super::Super::BeginPlay();

	const auto Root = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (!Root)
		UE_LOG(LogTemp, Fatal, TEXT("Gravity Component requires the actor's root is a type of Primitive Component!"));
	
	InitialiseSubstepTick(FPhysicsUtilities::GetBodyInstance(Root));
}

void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!Enabled)
		return;

	SubstepTick();
}

void UGravityComponent::OnSubstepTick(float DeltaTime, FBodyInstance* _)
{
	Super::OnSubstepTick(DeltaTime, _);
	
	SubstepBodyInstance->AddForce(GetForce(), false);
}
