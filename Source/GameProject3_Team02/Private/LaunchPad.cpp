#include "LaunchPad.h"
#include "GravityComponent.h"
#include "GravityOrientedActor.h"
#include "PlayerPawn.h"
#include "PlayerPhysicsActor.h"
#include "PowerUpHandler.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ALaunchPad::ALaunchPad()
{
	PrimaryActorTick.bCanEverTick = true;
	LaunchPad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mega Launch Pad"));
	RootComponent = LaunchPad;
}

void ALaunchPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerPhysicsActor::StaticClass()) && bIsActivated)
	{
		MegaPadMarker->SetVisibleFlag(false);
		LockedInOnLaunchPad();
		InGameUI->InGameUiInstance->ResetProgress();
		InGameUI->InGameUiInstance->HudPanel->SetVisibility(ESlateVisibility::Hidden);
		PlayerActor = Cast<APlayerPhysicsActor>(OtherActor);
		APowerUpHandler* PowerUpHandler = Cast<APowerUpHandler>(
			UGameplayStatics::GetActorOfClass(GetWorld(), APowerUpHandler::StaticClass()));
		InterpolationTime = 0.0f;
		
		if (GetWorldTimerManager().IsTimerActive(JumpDelay))
			GetWorldTimerManager().ClearTimer(JumpDelay);
		
		GetWorldTimerManager().SetTimer(JumpDelay, this, &ALaunchPad::Launch, 4.f, false);
		PlayerActor->GetGravity()->Enabled = false;
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PlayerController->GetPawn());
		//UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is null: %s"), PlayerPawn == nullptr ? "True" : "False");
		PlayerPawn->UnbindMoveInput();
		PlayerPawn->UnbindJumpInput();
		CurrentLocation = PlayerActor->GetActorLocation();
		Countdown();
		PowerUpHandler->ResetObjective();
	}
}

void ALaunchPad::Launch()
{
	Launched();
	InterpolationTime = 0.0f;
	bIsInterpolating = true;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PlayerController->GetPawn());
	if (bUseTransitionCamera)
	{
		TempCameraActor = GetWorld()->SpawnActor<ACameraActor>(
			ACameraActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		if (TempCameraActor)
		{
			TempCameraActor->GetCameraComponent()->SetWorldTransform(TransitionCameraComponent->GetComponentTransform());
			PlayerController->SetViewTargetWithBlend(TempCameraActor, LaunchCameraBlendTime);
		}
	}
	PlayerPawn->BindMoveInput();
	PlayerPawn->BindJumpInput();
}

void ALaunchPad::UpdateInterpolation(float DeltaTime)
{
	if (bIsInterpolating)
	{
		if (bUseTransitionCamera)
		{
			TempCameraActor->GetCameraComponent()->SetWorldLocation(PlayerActor->GetActorLocation() + TransitionCameraOffset);
			FVector CameraLocation = TempCameraActor->GetCameraComponent()->GetComponentLocation();
			FVector Target = PlayerActor->GetActorLocation();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target - CameraLocation).Rotator();

			TempCameraActor->GetCameraComponent()->SetWorldRotation(LookAtRotation);
		}

		FString LandingComponentName = LandingLocationComponent->GetName();
		FVector TargetLocationOffset = FVector(0, 0, LandingOffset);
		FRotator LandingPadRot = LandingLocationComponent->GetComponentRotation();
		TargetLocationOffset = LandingPadRot.RotateVector(TargetLocationOffset);

		FVector TargetLocation = LandingLocationComponent->GetRelativeLocation() + TargetLocationOffset;
		float InterpolationAlpha = FMath::Clamp(InterpolationTime / InterpolationDuration, 0.0f, 1.0f);
		FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, InterpolationAlpha);
		PlayerActor->SetActorLocation(NewLocation);

		if (InterpolationAlpha >= 1.0f)
		{
			InGameUI->InGameUiInstance->HudPanel->SetVisibility(ESlateVisibility::Visible);
			InGameUI->InGameUiInstance->Objective->SetVisibility(ESlateVisibility::Hidden);
			bIsInterpolating = false;
			PlayerActor->GetGravity()->Enabled = true;
			if (bUseTransitionCamera)
			{
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PlayerController->GetPawn());
				PlayerController->SetViewTargetWithBlend(PlayerPawn->GetCamera()->GetOwner(), LandingCameraBlendTime);
			}
			
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PlayerController->GetPawn());
			PlayerPawn->UnbindMoveInput();
			PlayerPawn->UnbindJumpInput();

			GetWorldTimerManager().SetTimer(LandDelayHandle, this, &ALaunchPad::OnLandDelayed, OnLandDelay, false);
		}
		else
		{
			InterpolationTime += DeltaTime;
		}
	}
}

void ALaunchPad::OnLandDelayed()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PlayerController->GetPawn());
	PlayerPawn->BindMoveInput();
	PlayerPawn->BindJumpInput();
}

void ALaunchPad::Countdown()
{
	GetWorldTimerManager().SetTimer(CountdownInterval, this, &ALaunchPad::Countdown, 1.0f, true);
	if (CountdownNumber == 3)
	{
		InGameUI->InGameUiInstance->MegaLaunchPadCountdownPanel->SetVisibility(ESlateVisibility::Visible);
	}
	InGameUI->InGameUiInstance->TakeoffCountdown->SetText(FText::FromString(FString::FromInt(CountdownNumber)));

	if (CountdownNumber != 0)
	{
		CountdownNumber -= 1;
	}
	else
	{
		InGameUI->InGameUiInstance->HudPanel->SetVisibility(ESlateVisibility::Hidden);
		InGameUI->InGameUiInstance->MegaLaunchPadCountdownPanel->SetVisibility(ESlateVisibility::Hidden);
		GetWorldTimerManager().ClearTimer(CountdownInterval);
	}
}

void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	InGameUI = Cast<AInGameUIHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), AInGameUIHandler::StaticClass()));
	ParentActor = GetAttachParentActor();
	
	if (ParentActor)
	{
		USceneComponent* ParentRootComponent = ParentActor->GetRootComponent();
		SetRootComponent(ParentRootComponent);
		LandingLocationComponent = Cast<ULandingLocationComponent>(
			ParentActor->GetComponentByClass(ULandingLocationComponent::StaticClass()));
		MegaPadMarker = Cast<UWidgetComponent>(ParentActor->GetComponentByClass(UWidgetComponent::StaticClass()));
		UE_LOG(LogTemp, Warning, TEXT("MegaPadMarker Name: %s"), *MegaPadMarker->GetName());
		if (bUseTransitionCamera)
		{
			TransitionCameraComponent = Cast<UCameraComponent>(ParentActor->FindComponentByClass<UCameraComponent>());
		}
		FString ParentActorName = ParentActor->GetName();
	}

	//PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AInGameUIHandler::StaticClass()));
}

void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsInterpolating)
	{
		UpdateInterpolation(DeltaTime);
	}
}
