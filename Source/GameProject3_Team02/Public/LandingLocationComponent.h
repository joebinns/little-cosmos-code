#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LandingLocationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT3_TEAM02_API ULandingLocationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	ULandingLocationComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
