// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravitySamplerComponent.h"
#include "GravityComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT3_TEAM02_API UGravityComponent : public UGravitySamplerComponent
{
	GENERATED_BODY()

public:	
	UGravityComponent();

	UPROPERTY()
	bool Enabled = true;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UPrimitiveComponent* Root;
};
