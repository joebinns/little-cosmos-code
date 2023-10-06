// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerPhysicsActor.h"
#include "Gravity/GravityComponent.h"
#include "Gravity/GravityFieldComponent.h"
#include "Utilities/MathsUtilities.h"
#include "Utilities/PhysicsUtilities.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Quat.h"

APlayerPhysicsActor::APlayerPhysicsActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Gravity = CreateDefaultSubobject<UGravityComponent>(TEXT("Gravity"));
    HoldTarget = CreateDefaultSubobject<USceneComponent>(TEXT("HoldTarget"));

	// Declare root
	RootComponent = Collider;

	// Setup collider
	Collider->SetSimulatePhysics(true);
	Collider->SetEnableGravity(false);

	// Setup hold pivot
	HoldTarget->SetupAttachment(Collider);
}

void APlayerPhysicsActor::BeginPlay()
{
	Super::BeginPlay();
	
	Gravity->OnPrimaryGravityFieldChangedDelegate.AddUniqueDynamic(this, &APlayerPhysicsActor::UpdateFocusedGravityField);
	
	InitialiseSubstepTick(FPhysicsUtilities::GetBodyInstance(GetRootComponent()));
}

void APlayerPhysicsActor::UpdateFocusedGravityField(UGravityFieldComponent* NewPrimaryGravityField)
{
	UGravityFieldComponent::FocusedGravityField = NewPrimaryGravityField;
	
	//UE_LOG(LogTemp, Warning, TEXT("Player Primary Gravity Field Changed!"));
}

#pragma region TICK
void APlayerPhysicsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdatePlatform(); // TODO: Should this be moved to SubstepTick?

	UpdateMoveState();

	UpdateJumpState(DeltaTime);
	
	SubstepTick();
}

#pragma region MOVING_PLATFORMS
void APlayerPhysicsActor::UpdatePlatform()
{
	SwitchPlatform();

	if (Platform == nullptr)
		return;

	// NOTE: If A is the transform to get from component space to world space,
	// then A^(-1) is the transform to get from world space to component space.
	// To get from space A to space B, use B^(-1) * A.

	// Calculate delta platform transform
	PlatformTransform = Platform->GetComponentTransform();
	FTransform DeltaPlatformTransform = PreviousPlatformTransform.Inverse() * PlatformTransform;
	FVector DeltaPlatformLocation = PlatformTransform.GetLocation() - PreviousPlatformTransform.GetLocation();
	// NOTE: For some reason, using the DeltaPlatformTransform.GetLocation() is not the same as this and caused weird results for rotated objects
	PreviousPlatformTransform = PlatformTransform;

	// Get player transform
	FTransform Transform = GetTransform();

	// Rotate location around platform pivot
	FVector Location = Transform.GetLocation();
	FVector PlatformPivot = PlatformTransform.GetLocation();
	FVector NewLocation = DeltaPlatformTransform.GetRotation() * (Location - PlatformPivot) + PlatformPivot;
	
	// Move location with platform
	NewLocation += DeltaPlatformLocation;

	// Rotate with platform
	FQuat Rotation = Transform.GetRotation();
	FQuat LocalDeltaPlatformRotation = Rotation * DeltaPlatformTransform.GetRotation() * Rotation.Inverse();
	FQuat NewRotation = Rotation * LocalDeltaPlatformRotation;

	// Apply
	Collider->SetWorldLocation(NewLocation);
	Collider->SetWorldRotation(NewRotation);
}

void APlayerPhysicsActor::SwitchPlatform()
{
	/* Platform Switching */
	Platform = GroundedHit.GetComponent();
	if (Platform == PreviousPlatform)
		return;
	PreviousPlatform = Platform;

	// When switching platform, reset transforms
	if (Platform != nullptr)
	{
		PreviousPlatformTransform = Platform->GetComponentTransform();
		//UE_LOG(LogTemp, Warning, TEXT("Attach!"));
	}
	else
	{
		PreviousPlatformTransform = FTransform::Identity;
		//UE_LOG(LogTemp, Warning, TEXT("De-attach!"));
	}
}
#pragma endregion

#pragma region MOVE_STATE
void APlayerPhysicsActor::UpdateMoveState()
{
	if (MoveState == EMoveState::ME_None)
	{
		if (MovementGoal != FVector::ZeroVector)
			SetMoveState(EMoveState::ME_Move);
	}
	else if (MoveState == EMoveState::ME_Move)
	{
		if (MovementGoal == FVector::ZeroVector)
			SetMoveState(EMoveState::ME_None);
	}
}

void APlayerPhysicsActor::SetMoveState(EMoveState State)
{
	MoveState = State;
	switch (MoveState)
	{
	case EMoveState::ME_None:
		OnCancelMoveDelegate.Broadcast();
		break;
	case EMoveState::ME_Move:
		OnBeginMoveDelegate.Broadcast();
		break;
	}
}
#pragma endregion

#pragma region JUMP
void APlayerPhysicsActor::QueueJump()
{
	TimeSinceJumpPressed = 0.f;
}

void APlayerPhysicsActor::AttemptStunt()
{
	if (JumpState != EJumpState::JE_Rise)
		return;

	// Queue stunt until time > stunt threshold
	IsStuntQueued = true;
}

void APlayerPhysicsActor::UpdateJumpState(float DeltaTime)
{
	// Update timers
	TimeSinceUngrounded = IsGrounded ? 0.f : TimeSinceUngrounded + DeltaTime;
	JumpTimeElapsed += DeltaTime;
	TimeSinceJumpPressed += DeltaTime;

	AttemptJump();

	// Set jump state
	if ((JumpState == EJumpState::JE_Rise) | (JumpState == EJumpState::JE_Stunt))
	{
		if (JumpState == EJumpState::JE_Rise)
		{
			if ((JumpTimeElapsed > StuntThreshold) & (IsStuntQueued))
			{
				SetJumpState(EJumpState::JE_Stunt);
				//UE_LOG(LogTemp, Warning, TEXT("Stunting!"));
			}
		}

		if (GetVerticalSpeed() < 0)
		{
			SetJumpState(EJumpState::JE_Fall);
			//UE_LOG(LogTemp, Warning, TEXT("Falling!"));
		}
	}
	
	if ((!WasGrounded) | (JumpState == EJumpState::JE_Fall)) // Must also check the jump state, as it otherwise breaks if the player completes a jump without becoming ungrounded
	{
		if (IsGrounded)
		{
			SetJumpState(EJumpState::JE_None);
			//UE_LOG(LogTemp, Warning, TEXT("Landed!"));
		}
	}
	WasGrounded = IsGrounded;
}

void APlayerPhysicsActor::SetJumpState(EJumpState State)
{
	JumpState = State;
	switch (JumpState)
	{
	case EJumpState::JE_None:
		JumpIndex = 0;
		Gravity->ForceFactor = 1.f;
		OnLandDelegate.Broadcast();
		break;
	case EJumpState::JE_Rise:
		if (JumpIndex == 1)
		{
			Gravity->ForceFactor = JumpRiseGravityFactor;
			OnJumpDelegate.Broadcast();
		}
		else
		{
			Gravity->ForceFactor = DoubleJumpRiseGravityFactor;
			OnDoubleJumpDelegate.Broadcast();
		}
		break;
	case EJumpState::JE_Fall:
		Gravity->ForceFactor = JumpFallGravityFactor;
		break;
	case EJumpState::JE_Stunt:
		Gravity->ForceFactor = JumpStuntGravityFactor;
		IsStuntQueued = false;
		break;
	}
}

void APlayerPhysicsActor::AttemptJump()
{
	if (TimeSinceJumpPressed > JumpInputBuffer)
		return;
	if (JumpIndex >= 2)
		return;

	float InitialJumpSpeed;
	int WouldBeJumpIndex;
	if (TimeSinceUngrounded < CoyoteTime)
	{
		if (JumpState != EJumpState::JE_None)
			return;
		InitialJumpSpeed = JumpInitialVerticalSpeed;
		WouldBeJumpIndex = 1;
	}
	else
	{
		InitialJumpSpeed = DoubleJumpInitialVerticalSpeed;
		WouldBeJumpIndex = 2;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Vertical Speed: %f"), GetVerticalSpeed());
	//UE_LOG(LogTemp, Warning, TEXT("Initial Jump Speed: %f"), InitialJumpSpeed);

	// Prevent jumping if the current vertical speed is significantly less than initial jump speed
	if (GetVerticalSpeed() * 0.5f > InitialJumpSpeed)
		return;

	// Only update the jump index if the jump is going to be performed
	JumpIndex = WouldBeJumpIndex;

	// Track how much of the jumps duration has elapsed
	TimeSinceJumpPressed = INFINITY;
	JumpTimeElapsed = 0.f;

	// Set the jump state
	SetJumpState(EJumpState::JE_Rise);

	Jump(InitialJumpSpeed);
}

void APlayerPhysicsActor::Jump(float InitialJumpSpeed)
{
	// Calculate the vertical speed prior to jumping
	FVector GravityDirection = Gravity->GetForceDirection();

	// Set the height to be the ride height... (not sure if there's a less snappy way of going about this...)
	Collider->AddRelativeLocation(GetRideHeightError() * GravityDirection);

	// Set (initial) velocity
	FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
	FVector Velocity = DeltaGravityRotation.UnrotateVector(GetVelocity());
	Velocity.Z = InitialJumpSpeed;
	Velocity = DeltaGravityRotation.RotateVector(Velocity);

	Collider->SetPhysicsLinearVelocity(Velocity);

	//UE_LOG(LogTemp, Warning, TEXT("Jump!"));
}
#pragma endregion

UGravityComponent* APlayerPhysicsActor::GetGravity()
{
	return Gravity;
}

float APlayerPhysicsActor::GetVerticalSpeed()
{
	// Calculate velocity of player relative to platform
	FVector Velocity = SubstepBodyInstance->GetUnrealWorldVelocity();
	float VerticalSpeed = FVector::DotProduct(-Gravity->GetForceDirection(), Velocity);
	return VerticalSpeed;
}

float APlayerPhysicsActor::GetRideHeightError()
{
	// Get ride height error
	float RideHeightError = GroundedHit.Distance - RideHeight;
	return IsGrounded ? RideHeightError : 0.f;
}

#pragma endregion

#pragma region SUBSTEP_TICK
void APlayerPhysicsActor::OnSubstepTick(float DeltaTime, FBodyInstance* _)
{
	// This function now gets called during the physics tick.
	// If any "additional" physics frames get inserted,
	// this function will get called multiple times

	GroundCheck();
	
	Move(DeltaTime);
	
	if (IsGrounded & (JumpState != EJumpState::JE_Rise))
		MaintainHeight();
	
	MaintainErect();
}

void APlayerPhysicsActor::GroundCheck()
{
	FVector TraceStart = SubstepBodyInstance->GetUnrealWorldTransform().GetLocation();
	FVector TraceEnd = TraceStart + Gravity->GetForceDirection() * TraceDistance;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(GroundedHit, TraceStart, TraceEnd, GroundCheckTraceChannel, QueryParams);

#if WITH_EDITORONLY_DATA
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, GroundedHit.bBlockingHit ? FColor::Green : FColor::Red, false, 5.0f,
				  0, 1.0f);
#endif

	IsGrounded = GroundedHit.bBlockingHit && IsValid(GroundedHit.GetActor());
}

void APlayerPhysicsActor::Move(float DeltaTime)
{
	// Get acceleration magnitude from evaluating a curve
	FVector CurrentVelocity = SubstepBodyInstance->GetUnrealWorldVelocity();
	float VelocityAlignment = FVector::DotProduct(MovementGoal, CurrentVelocity.GetSafeNormal());
	float AccelerationMagnitude = AccelerationCurve->GetFloatValue(VelocityAlignment) * AccelerationFactor;

	// Calculate Target Velocity
	FVector GoalVelocity = MovementGoal * MaxSpeedFactor;
	TargetVelocity = FMathsUtilities::MoveTowards(TargetVelocity, GoalVelocity, AccelerationMagnitude * DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Target Velocity: %s"), *TargetVelocity.ToString());

	// Calculate acceleration to apply
	FVector AccelerationToApply = (TargetVelocity - CurrentVelocity) / DeltaTime;

	// Ensure acceleration to apply is zero on the 'up' vector
	FVector GravityDirection = Gravity->GetForceDirection();
	FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
	AccelerationToApply = DeltaGravityRotation.UnrotateVector(AccelerationToApply);
	AccelerationToApply.Z = 0.f;
	AccelerationToApply = DeltaGravityRotation.RotateVector(AccelerationToApply);

	//UE_LOG(LogTemp, Warning, TEXT("Acceleration To Apply: %s"), *AccelerationToApply.ToString());

	// Clamp acceleration from evaluating another curve
	float MaximumAcceleration = MaximumAccelerationCurve->GetFloatValue(VelocityAlignment) * MaxAccelerationFactor;
	AccelerationToApply = AccelerationToApply.GetClampedToSize(0.f, MaximumAcceleration);

	// Apply force
	FVector ForceToApply = AccelerationToApply * Collider->GetMass();
	FVector PositionToApplyForce = SubstepBodyInstance->GetUnrealWorldTransform().GetLocation() - GravityDirection * LeanFactor;
	SubstepBodyInstance->AddForceAtPosition(ForceToApply, PositionToApplyForce, false);

	//UE_LOG(LogTemp, Warning, TEXT("Movement Force Applied: %s"), *ForceToApply.ToString());

	// TODO: Fix problem where the character has non-zero vertical speed when moving around round gravity fields...
}

void APlayerPhysicsActor::MaintainHeight()
{
	// Calculate velocity of player relative to platform
	float VerticalSpeed = GetVerticalSpeed();

	// Get ride height error
	float RideHeightError = GetRideHeightError();

	// Calculate restoring force
	float RestoringForceMagnitude = RideHeightError * RideHeightStiffness + VerticalSpeed * RideHeightDamper;
	FVector RestoringForce = RestoringForceMagnitude * Gravity->GetForceDirection();

	// Subtract gravitational force from restoring force
	FVector GravityCorrectedRestoringForce = RestoringForce - Gravity->GetForce();

	// Apply force
	SubstepBodyInstance->AddForce(GravityCorrectedRestoringForce, false);

	// Apply force to objects beneath
	if (!IsGrounded)
		return;
	
	if (Platform)
	{
		if (Platform->IsSimulatingPhysics())
			Platform->GetBodyInstance()->AddForceAtPosition(-RestoringForce * GroundedHit.ImpactNormal, GroundedHit.ImpactPoint, false);
	}
}

void APlayerPhysicsActor::MaintainErect()
{
	FTransform SubstepTransform = SubstepBodyInstance->GetUnrealWorldTransform();
	FQuat DeltaForwardRotation = FQuat::FindBetweenNormals(SubstepTransform.GetRotation().GetForwardVector(), FacingGoal);
	FQuat DeltaUpRotation = FQuat::FindBetweenNormals(-SubstepTransform.GetRotation().GetUpVector(), ErectGoal);
	FQuat DeltaRotation = DeltaForwardRotation * DeltaUpRotation;

	// Convert Delta Rotation to it's angle and axis
	FVector DeltaRotationAxis;
	float DeltaRotationAngle;
	DeltaRotation.ToAxisAndAngle(DeltaRotationAxis, DeltaRotationAngle);

	// Calculate and apply torque
	FVector AngularVelocity = SubstepBodyInstance->GetUnrealWorldAngularVelocityInRadians();
	FVector Torque = (DeltaRotationAxis * DeltaRotationAngle * ErectStiffness) - (AngularVelocity * ErectDamper);
	SubstepBodyInstance->AddTorqueInRadians(Torque, false);

	/*
	if (IsGrounded)
	{
		// Correct rotation to be 'straight up'
		DeltaUpRotation = FQuat::FindBetweenNormals(-GetActorUpVector(), ErectGoal);
		Collider->AddWorldRotation(DeltaUpRotation);
	}
	*/
}

#pragma endregion

USceneComponent* APlayerPhysicsActor::GetHoldTarget()
{
	return HoldTarget;
}

