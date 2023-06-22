// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InGameUIHandler.h"
#include "PlayerPhysicsActor.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

// Forward declarations
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UCameraComponent;
class USpringArmComponent;
class AFollowActor;
struct FInputBindingHandle;

UCLASS()
class GAMEPROJECT3_TEAM02_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();
	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	APlayerPhysicsActor* PhysicsActor;

	UFUNCTION(BlueprintCallable)
	APlayerPhysicsActor* GetPhysicsActor();

	UFUNCTION(BlueprintCallable)
	UCameraComponent* GetCamera();

#pragma region INPUT

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Camera")
	float YawSensitivity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Controls|Camera")
	float PitchSensitivity = 1.f;
	
	FInputBindingHandle* MoveTriggeredBinding;
	FInputBindingHandle* MoveCompletedBinding;

	UFUNCTION()
	void BindMoveInput();
	UFUNCTION()
	void UnbindMoveInput();

	FInputBindingHandle* JumpStartedBinding;
	FInputBindingHandle* JumpCompletedBinding;

	UFUNCTION()
	void BindJumpInput();
	UFUNCTION()
	void UnbindJumpInput();

	FInputBindingHandle* LookTriggeredBinding;
	
	UFUNCTION()
	void BindLookInput();
	UFUNCTION()
	void UnbindLookInput();
    	
    UFUNCTION()
    void BindPauseInput();

#pragma endregion 
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PawnClientRestart() override;

	APlayerController* PlayerController;

#pragma region INPUT

	UFUNCTION()
	void UpdateInputAxes();
	
	FVector Forward;
	FVector Up;
	FVector Right;
	
	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	UInputAction* LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
    UInputAction* PauseAction;

	// Input Mappings
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Mappings")
	UInputMappingContext* GameMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Mappings")
	int32 GameMappingPriority = 0;

	UPROPERTY()
	UEnhancedInputComponent* PlayerEnhancedInputComponent;
	
	// Input Functions
	void Move(const FInputActionValue& Value);
	void CancelMove(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void CancelJump(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Pause(const FInputActionValue& Value);

	UFUNCTION()
	void SetMovementInputDirection(FVector Direction);

#pragma endregion

#pragma region CAMERA

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	FRotator InitialSpringArmRotation;

	UPROPERTY(EditAnywhere)
	USceneComponent* SpringArmPivot;
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere)
	UArrowComponent* TargetCameraDirection;
	
	/* The lower and upper limits of yaw, in degrees. */
	UPROPERTY(EditAnywhere, Category="Controls|Camera")
	FVector2D PitchRange = FVector2D(-75.f, 25.f);

	UPROPERTY(EditAnywhere, Category="Controls|Camera")
	bool ShouldInvertPitchInput = false;

	UPROPERTY(EditAnywhere, Category="Camera|Custom Lag")
	float MaximumCameraLagDistance = 100.f;

	UPROPERTY(EditAnywhere, Category="Camera|Custom Lag")
	float CameraLocationLagSpeed = 2.f;
	
	UPROPERTY(EditAnywhere, Category="Camera|Custom Lag")
	float CameraRotationLagSpeed = 4.f;

	

#pragma endregion

#pragma region PHYSICS_ACTOR

	UPROPERTY(EditDefaultsOnly, Category="PhysicsActor")
	TSubclassOf<APlayerPhysicsActor> PhysicsActorBP;

	UPROPERTY(EditAnywhere)
	USceneComponent* Follower;

	UFUNCTION()
	void SpawnPhysicsActor();

	UFUNCTION()
	void FollowPhysicsActor();

	UFUNCTION()
	void UpdateRotation();

	UFUNCTION()
	void CameraLag(float DeltaTime);

	UFUNCTION()
	void InitialiseRotation();

	UFUNCTION()
	FVector InputSpaceToWorldSpace(FVector Vector);

#pragma endregion
	
	UFUNCTION()
	void Debug();

private:
	UEnhancedInputLocalPlayerSubsystem* SubSystem;

	FVector PreviousGravityDirection;
	FQuat TargetRotation;

	AInGameUIHandler* UIHandler;
	
};
