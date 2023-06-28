// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GravityComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create components
	Follower = CreateDefaultSubobject<USceneComponent>(TEXT("PhysicsActorFollower"));
	SpringArmPivot = CreateDefaultSubobject<USceneComponent>(TEXT("SpringArmPivot"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TargetCameraDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetCameraDirection"));

	// Declare root
	RootComponent = Follower;

	// Setup camera
	SpringArmPivot->SetupAttachment(Follower);
	SpringArmPivot->SetUsingAbsoluteLocation(true);
	SpringArmPivot->SetUsingAbsoluteRotation(true);
	SpringArm->SetupAttachment(SpringArmPivot);
	SpringArm->SetRelativeLocation(FVector::UpVector * 50.f);
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->TargetArmLength = 1080.f;
	Camera->SetupAttachment(SpringArm);
	Camera->FieldOfView = 60.f;
	

	TargetCameraDirection->SetupAttachment(Follower);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	SpawnPhysicsActor();
	
	InitialiseRotation();
	
	UIHandler = Cast<AInGameUIHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), AInGameUIHandler::StaticClass()));
}

void APlayerPawn::SpawnPhysicsActor()
{
	FActorSpawnParameters SpawnParams;
	PhysicsActor = GetWorld()->SpawnActor<APlayerPhysicsActor>(
		PhysicsActorBP ? PhysicsActorBP : APlayerPhysicsActor::StaticClass(), GetTransform(), SpawnParams);
}

void APlayerPawn::InitialiseRotation()
{
	PreviousGravityDirection = PhysicsActor->GetGravity()->SampleForceDirection();
	FQuat DeltaRotation = FQuat::FindBetweenNormals(FVector::DownVector, PreviousGravityDirection);
	SetActorRotation(DeltaRotation);
	TargetRotation = DeltaRotation;
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FollowPhysicsActor();

	UpdateRotation();

	CameraLag(DeltaTime);

	UpdateInputAxes();

	Debug();
}

void APlayerPawn::UpdateInputAxes()
{
	// Update the forward and right axes for input
	Up = PhysicsActor->GetGravity()->SampleForceDirection();
	Forward = UKismetMathLibrary::ProjectVectorOnToPlane(TargetCameraDirection->GetForwardVector(), Up);
	Forward = Forward.GetSafeNormal();
	Right = Forward.Cross(Up);
	PhysicsActor->ErectGoal = Up;
}

void APlayerPawn::FollowPhysicsActor()
{
	FVector PhysicsActorLocation = PhysicsActor->GetActorLocation();
	SetActorLocation(PhysicsActorLocation);
}

void APlayerPawn::UpdateRotation()
{
	// Slerp current rotation to desired rotation, such that player down is in the direction of gravity
	FVector GravityDirection = PhysicsActor->GetGravity()->SampleForceDirection();
	FQuat DeltaCurrentRotation = FQuat::FindBetweenNormals(PreviousGravityDirection, GravityDirection);
	PreviousGravityDirection = GravityDirection;
	TargetRotation = DeltaCurrentRotation * TargetRotation;
	
	SetActorRotation(TargetRotation);
}

void APlayerPawn::CameraLag(float DeltaTime)
{
	// Ease camera transform towards target
	FVector LaggedCameraLocation = FMath::VInterpTo(SpringArmPivot->GetComponentLocation(), Follower->GetComponentLocation(), DeltaTime, CameraLocationLagSpeed);
	FQuat LaggedCameraRotation = FMath::QInterpTo(SpringArmPivot->GetComponentRotation().Quaternion(), Follower->GetComponentRotation().Quaternion(), DeltaTime, CameraRotationLagSpeed);

	// Clamp camera location
	LaggedCameraLocation = (LaggedCameraLocation - Follower->GetComponentLocation()).GetClampedToSize(0.f, MaximumCameraLagDistance) + Follower->GetComponentLocation();

	SpringArmPivot->SetWorldLocation(LaggedCameraLocation);
	SpringArmPivot->SetWorldRotation(LaggedCameraRotation);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Make sure that we are using a UEnhancedInputComponent; if not, the project is not configured correctly.
	PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerEnhancedInputComponent == nullptr)
		return;

	// TODO: Fix removing bindings, so that this can be done dynamically in the BindInput methods
	if (MoveAction)
		MoveTriggeredBinding = &PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
		MoveCompletedBinding = &PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerPawn::CancelMove);
	if (JumpAction)
		JumpStartedBinding = &PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerPawn::Jump);
		JumpCompletedBinding = &PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerPawn::CancelJump);
	if (LookAction)
		LookTriggeredBinding = &PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerPawn::Look);

	BindMoveInput();
	BindJumpInput();
	BindLookInput();
	BindPauseInput();
}

// TODO: Fix RemoveBinding() usage
#pragma region INPUT_BINDINGS
void APlayerPawn::BindMoveInput()
{
	if (!MoveAction)
		return;
	
	//MoveTriggeredBinding = &PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
	//MoveCompletedBinding = &PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerPawn::CancelMove);
	IsMoveBound = true;

	UE_LOG(LogTemp, Warning, TEXT("Move input binded"));
	
}

void APlayerPawn::UnbindMoveInput()
{
	if (!MoveAction)
		return;
	
	//PlayerEnhancedInputComponent->RemoveBinding(*MoveTriggeredBinding);
	//PlayerEnhancedInputComponent->RemoveBinding(*MoveCompletedBinding);
	IsMoveBound = false;
	
	SetMovementInputDirection(FVector(0.f, 0.f, 0.f));

	UE_LOG(LogTemp, Warning, TEXT("Move input unbinded"));
}

void APlayerPawn::BindJumpInput()
{
	if (!JumpAction)
		return;

	//JumpStartedBinding = &PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerPawn::Jump);
	//JumpCompletedBinding = &PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerPawn::CancelJump);
	IsJumpBound = true;

	UE_LOG(LogTemp, Warning, TEXT("Jump input binded"));
}

void APlayerPawn::UnbindJumpInput()
{
	if (!JumpAction)
		return;

	//PlayerEnhancedInputComponent->RemoveBinding(*JumpStartedBinding);
	//PlayerEnhancedInputComponent->RemoveBinding(*JumpCompletedBinding);
	IsJumpBound = false;
	
	PhysicsActor->SetJumpState(EJumpState::JE_None);

	UE_LOG(LogTemp, Warning, TEXT("Jump input unbinded"));
}

void APlayerPawn::BindLookInput()
{
	if (!LookAction)
		return;

	//LookTriggeredBinding = &PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerPawn::Look);
	IsLookBound = true;

	UE_LOG(LogTemp, Warning, TEXT("Look input binded"));
}

void APlayerPawn::UnbindLookInput()
{
	if (!LookAction)
		return;

	//PlayerEnhancedInputComponent->RemoveBinding(*LookTriggeredBinding);
	IsLookBound = false;

	UE_LOG(LogTemp, Warning, TEXT("Look input unbinded"));
}

void APlayerPawn::BindPauseInput()
{
	if(!PauseAction)
		return;

	PlayerEnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &APlayerPawn::Pause);
}

#pragma endregion 

void APlayerPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (PlayerController == nullptr)
		PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return;

	if (SubSystem == nullptr)
		SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
	SubSystem->ClearAllMappings();

	// Add each mapping context, along with their priority values. Higher values out-prioritize lower values.
	SubSystem->AddMappingContext(GameMappingContext, GameMappingPriority);
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	if (!IsMoveBound)
		return;
	
	SetMovementInputDirection(Value.Get<FVector>().GetSafeNormal());
}

void APlayerPawn::CancelMove(const FInputActionValue& Value)
{
	if (!IsMoveBound)
		return;
	
	SetMovementInputDirection(FVector(0.f, 0.f, 0.f));
}

FVector APlayerPawn::InputSpaceToWorldSpace(FVector Vector)
{
	FVector WorldVector = Forward * Vector.X + Right * Vector.Y;
	return WorldVector;
}

void APlayerPawn::SetMovementInputDirection(FVector Input)
{
	PhysicsActor->MovementGoal = InputSpaceToWorldSpace(Input);
	PhysicsActor->FacingGoal = InputSpaceToWorldSpace(Input);

	//UE_LOG(LogTemp, Warning, TEXT("Input: %s"), *Input.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("World Input: %s"), *WorldInput.ToString());
}

void APlayerPawn::Jump(const FInputActionValue& Value)
{
	if (!IsJumpBound)
		return;
	
	PhysicsActor->QueueJump();
}

void APlayerPawn::CancelJump(const FInputActionValue& Value)
{
	if (!IsJumpBound)
		return;
	
	PhysicsActor->AttemptStunt();
}

void APlayerPawn::Look(const FInputActionValue& Value)
{
	if (!IsLookBound)
		return;
	
	FVector2D Look = Value.Get<FVector2D>();
	if (Look != FVector2D::ZeroVector)
	{
		AddControllerYawInput(Look.X * YawSensitivity);

		// Lock the pitch input so that control yaw is within a range
		FRotator ControlRotation = GetControlRotation();
		float NewPitch = ControlRotation.Pitch + Look.Y * PitchSensitivity * (ShouldInvertPitchInput ? -1.f : 1.f);
		float ClampedPitch = UKismetMathLibrary::ClampAngle(NewPitch, PitchRange.X, PitchRange.Y);
		ControlRotation.Pitch = ClampedPitch;
		Controller->SetControlRotation(ControlRotation);

		// Apply control rotation to spring arm
		SpringArm->SetRelativeRotation(GetControlRotation());

		// Apply control rotation to target camera direction
		FQuat WorldControlRotation = FQuat::MakeFromRotator(GetActorRotation()) * FQuat::MakeFromRotator(GetControlRotation()); // Convert control rotation from player space to world 
		TargetCameraDirection->SetWorldRotation(WorldControlRotation);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Look: %f"), Look.X);
	//UE_LOG(LogTemp, Warning, TEXT("Control Rotation: %s"), *GetControlRotation().Euler().ToString());
}

void APlayerPawn::Pause(const FInputActionValue& Value)
{
	if (UIHandler == nullptr)
		return;
	
	UIHandler->TogglePause();
}

void APlayerPawn::Debug()
{
}

APlayerPhysicsActor* APlayerPawn::GetPhysicsActor()
{
	return PhysicsActor;
}

UCameraComponent* APlayerPawn::GetCamera()
{
	return Camera;
}


