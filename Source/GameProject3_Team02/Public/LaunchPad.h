#pragma once

#include "CoreMinimal.h"
#include "GravityOrientedActor.h"
#include "InGameUIHandler.h"
#include "InGameUIWidget.h"
#include "LandingLocationComponent.h"
#include "PlayerPhysicsActor.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

class APlayerPawn;
UCLASS()
class GAMEPROJECT3_TEAM02_API ALaunchPad : public AActor
{
	GENERATED_BODY()

public:
	ALaunchPad();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LaunchPad;

	FVector CurrentLocation;

	AActor* ParentActor;

	ULandingLocationComponent* LandingLocationComponent;

	FTimerHandle JumpDelay;

	FTimerHandle CountdownInterval;

	int CountdownNumber = 3;

	UPROPERTY()
	int LaunchPadID;


	UPROPERTY()
	UWidgetComponent* MegaPadMarker;
	
	UFUNCTION()
	void Launch();


	UFUNCTION(BlueprintImplementableEvent)
	void LockedInOnLaunchPad();

	UFUNCTION(BlueprintImplementableEvent)
	void Launched();

#pragma region BOOLS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
    bool bIsActivated;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	bool bIsInterpolating = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	bool bUseTransitionCamera = false;


#pragma endregion

#pragma region DESIGNER-DESIRABLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	float InterpolationDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	float LandingOffset = 100.0f;
	
	UCameraComponent* TransitionCameraComponent;
	ACameraActor* TempCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	FVector TransitionCameraOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	float LaunchCameraBlendTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	float LandingCameraBlendTime = 1.0f;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LAUNCHPAD PROPERTIES")
	// float InterpolationSpeed = 5.0f;
#pragma endregion

#pragma region REFS
	//APlayerPawn* PlayerPawn;
	APlayerPhysicsActor* PlayerActor;
	AInGameUIHandler* InGameUI;
#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void UpdateInterpolation(float DeltaTime);
	void Countdown();
	float InterpolationTime = 0.0f;
#pragma endregion

	virtual void Tick(float DeltaTime) override;

protected:
	FTimerHandle LandDelayHandle;

	UPROPERTY(EditAnywhere)
	float OnLandDelay = 2.f;
	
	void OnLandDelayed();
	
	virtual void BeginPlay() override;
};
