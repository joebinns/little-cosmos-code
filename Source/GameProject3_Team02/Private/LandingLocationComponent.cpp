#include "LandingLocationComponent.h"

ULandingLocationComponent::ULandingLocationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void ULandingLocationComponent::BeginPlay()
{
	Super::BeginPlay();
}


void ULandingLocationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

