#include "LocationMarkerComponent.h"

ULocationMarkerComponent::ULocationMarkerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULocationMarkerComponent::BeginPlay()
{
	Super::BeginPlay();
}


void ULocationMarkerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

