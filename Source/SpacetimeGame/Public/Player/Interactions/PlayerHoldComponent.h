// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/Hold/HoldableInterface.h"
#include "SubstepTick/SubstepTickInterface.h"
#include "PlayerHoldComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACETIMEGAME_API UPlayerHoldComponent : public UActorComponent, public ISubstepTickInterface
{
	GENERATED_BODY()

public:	
	UPlayerHoldComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

	void AttemptGrab();
	
	void AttemptRelease();

#pragma region TRACE

	UPROPERTY()
	FVector TraceDirection;
	
	UPROPERTY()
	float MaxReach = 200.f;

#pragma endregion
	
	UPROPERTY()
	USceneComponent* HoldTarget;

protected:
	
	IHoldableInterface* Held;

#pragma region TRACE

	IHoldableInterface* PotentialHoldable;
	
	UPROPERTY(BlueprintReadOnly)
	FHitResult HoldableTraceHit;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> HoldableTraceChannel = ECC_Visibility;
	
	void UpdatePotentialHoldable();

#pragma endregion TRACE
	
	virtual void OnSubstepTick(float DeltaTime, FBodyInstance* _) override;

	void UpdateEquilibriums();

private:
#pragma region TRACE
	IHoldableInterface* PreviousPotentialHoldable;
#pragma endregion
	
};

