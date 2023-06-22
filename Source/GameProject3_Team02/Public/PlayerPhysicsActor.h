// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerPhysicsActor.generated.h"

class UArrowComponent;
class UGravityComponent;

UENUM(BlueprintType)
enum class EJumpState : uint8
{
	JE_None UMETA(DisplayName="None"),
	JE_Rise UMETA(DisplayName="Rise"),
	JE_Fall UMETA(DisplayName="Fall"),
	JE_Stunt UMETA(DisplayName="Stunt")
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	ME_None UMETA(DisplayName="None"),
	ME_Move UMETA(DisplayName="Move")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoubleJumpSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLandSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelMoveSignature);

UCLASS()
class GAMEPROJECT3_TEAM02_API APlayerPhysicsActor : public AActor
{
	GENERATED_BODY()

public:
	APlayerPhysicsActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsOnRail = false;

	int JumpIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	bool IsGrounded;

	UPROPERTY(BlueprintAssignable)
	FOnJumpSignature OnBeginMoveDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnJumpSignature OnCancelMoveDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnJumpSignature OnJumpDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDoubleJumpSignature OnDoubleJumpDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnLandSignature OnLandDelegate;

	UFUNCTION(BlueprintCallable)
	UGravityComponent* GetGravity();

	UFUNCTION()
	void QueueJump();

	UFUNCTION()
	void AttemptStunt();

	UFUNCTION(BlueprintCallable)
	void Jump(float InitialJumpSpeed);

	UFUNCTION(BlueprintCallable)
	void SetJumpState(EJumpState State);

	UFUNCTION()
	void SetMoveState(EMoveState State);

	FVector ErectGoal = FVector::ZeroVector;
	FVector MovementGoal = FVector::ZeroVector;
	FVector FacingGoal = FVector::ZeroVector;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

#pragma region COLLIDER

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Collider;

#pragma endregion

#pragma region GRAVITY

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGravityComponent* Gravity;

	UFUNCTION()
	float GetVerticalSpeed();

#pragma endregion

#pragma region GROUND_CHECK

    UPROPERTY(BlueprintReadOnly)
	FHitResult GroundedHit;

	/* The trace distance of the ground check. Should be greater than the ride height */
	UPROPERTY(EditAnywhere, Category="Ground Check")
	float TraceDistance = 150.f;

	/* The trace channel of the ground check */
	UPROPERTY(EditAnywhere, Category="Ground Check")
	TEnumAsByte<ECollisionChannel> GroundCheckTraceChannel = ECC_Visibility;

	void GroundCheck();

#pragma endregion

#pragma region MOVEMENT

	/* The maximum speed factor for movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
	float MaxSpeedFactor = 1000.f;

	/* The acceleration factor for movement */
	UPROPERTY(EditAnywhere, Category="Movement")
	float AccelerationFactor = 10000.f;

	/* The maximum acceleration factor for movement */
	UPROPERTY(EditAnywhere, Category="Movement")
	float MaxAccelerationFactor = 5000.f;

	/* The acceleration curve for the dot product of the velocity. Don't touch or Joe will be mad! */
	UPROPERTY(EditAnywhere, Category="Movement")
	UCurveFloat* AccelerationCurve;

	/* The maximum acceleration curve for the dot product of the velocity. Don't touch or Joe will be mad! */
	UPROPERTY(EditAnywhere, Category="Movement")
	UCurveFloat* MaximumAccelerationCurve;

	UFUNCTION()
	void Move(float DeltaTime);

	UFUNCTION()
	void UpdateMoveState();

#pragma endregion

#pragma region RIDE_HEIGHT_OSCILLATOR

	/* The equilibrium height of the ride height oscillator */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="RideHeight")
	float RideHeight = 120.f;

	/* The stiffness of the ride height oscillator */
	UPROPERTY(EditAnywhere, Category="RideHeight")
	float RideHeightStiffness = 150000.f;

	/* The damper of the ride height oscillator */
	UPROPERTY(EditAnywhere, Category="RideHeight")
	float RideHeightDamper = 10000.f;

	UFUNCTION()
	void MaintainHeight(float DeltaTime);

	UFUNCTION()
	float GetRideHeightError();

#pragma endregion

#pragma region ERECT_OSCILLATOR

	/* The stiffness of the erect oscillator */
	UPROPERTY(EditAnywhere, Category="Erect")
	float ErectStiffness = 100000000.f;

	/* The damper applied to the erect oscillator */
	UPROPERTY(EditAnywhere, Category="Erect")
	float ErectDamper = 10000000.f;

	UFUNCTION()
	void MaintainErect(float DeltaTime);

#pragma endregion

#pragma region JUMP

	/* The vertical speed used to launch the player when jumping */
	UPROPERTY(EditAnywhere, Category="Jump")
	float JumpInitialVerticalSpeed = 1500.f;

	/* The vertical speed used to re-launch the player when double jumping */
	UPROPERTY(EditAnywhere, Category="Jump")
	float DoubleJumpInitialVerticalSpeed = 1000.f;

	/* The factor by which the magnitude of gravity is multiplied when on the rise curve of a jump */
	UPROPERTY(EditAnywhere, Category="Jump")
	float JumpRiseGravityFactor = 0.5f;

	/* The factor by which the magnitude of gravity is multiplied when on the rise curve of a double jump */
	UPROPERTY(EditAnywhere, Category="Jump")
	float DoubleJumpRiseGravityFactor = 0.75f;

	/* The factor by which the magnitude of gravity is multiplied when on the fall curve of a jump */
	UPROPERTY(EditAnywhere, Category="Jump")
	float JumpFallGravityFactor = 1.5f;

	/* The factor by which the magnitude of gravity is multiplied when a jump is prematurely stunted */
	UPROPERTY(EditAnywhere, Category="Jump")
	float JumpStuntGravityFactor = 2.f;

	/* The minimum amount of time after which a jump can be stunted */
	UPROPERTY(EditAnywhere, Category="Jump")
	float StuntThreshold = 0.15f;

	/* The maximum amount of time after becoming un-grounded that a jump will still be registered */
	UPROPERTY(EditAnywhere, Category="Jump")
	float CoyoteTime = 0.2f;

	/* The maximum amount of time up to which the next jump's input will be buffered prior to landing */
	UPROPERTY(EditAnywhere, Category="Jump")
	float JumpInputBuffer = 0.1f;

	UFUNCTION()
	void AttemptJump();

	UFUNCTION()
	void UpdateJumpState(float DeltaTime);

#pragma endregion

#pragma region MOVING_PLATFORMS

	UPROPERTY()
	UPrimitiveComponent* Platform;

	UPROPERTY()
	UPrimitiveComponent* PreviousPlatform;

	UFUNCTION()
	void SwitchPlatform();

	UFUNCTION()
	void UpdatePlatform();

#pragma endregion

private:
	// Movement
	FVector TargetVelocity;
	EMoveState MoveState = EMoveState::ME_None;

	// Jump
	bool IsStuntQueued = false;
	bool WasGrounded;
	float JumpTimeElapsed = 0.f;
	float TimeSinceUngrounded = 0.f;
	float TimeSinceJumpPressed = 100.f;
	EJumpState JumpState = EJumpState::JE_None;

	// Platform
	FTransform PlatformTransform;
	FTransform PreviousPlatformTransform;

	FVector PreviousGravityDirection;
	FVector PreviousForwardDirection;
};
