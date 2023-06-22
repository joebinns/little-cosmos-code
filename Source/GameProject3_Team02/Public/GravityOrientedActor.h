// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GravityOrientedActor.generated.h"

class UEditorGravitySamplerComponent;

UCLASS()
class GAMEPROJECT3_TEAM02_API AGravityOrientedActor : public AActor
{
	GENERATED_BODY()
	
public:
	AGravityOrientedActor();

#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif
	
protected:
	UPROPERTY(EditAnywhere, Category="GravityOrientedActor")
	UEditorGravitySamplerComponent* EditorGravitySampler;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="GravityOrientedActor")
	USceneComponent* GravityOriented;

	UPROPERTY(EditAnywhere, Category="GravityOrientedActor")
	bool IsOrientationInverted = false;

	void Orient();
	
	UFUNCTION(CallInEditor, Category="GravityOrientedActor")
	void ResetOrientation();
	
	UFUNCTION(CallInEditor, Category="GravityOrientedActor")
	void InvertOrientation();

private:
	FVector PreviousGravityDirection;

};
