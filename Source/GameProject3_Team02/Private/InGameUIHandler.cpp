#include "InGameUIHandler.h"
#include "GP3_GameInstance.h"
#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"

AInGameUIHandler::AInGameUIHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInGameUIHandler::P_PressQuit()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

void AInGameUIHandler::TogglePause()
{
	UE_LOG(LogTemp, Warning, TEXT("Stunting!"));
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	bPaused = !bPaused;
	if(bPaused)
	{
		InGameUiInstance->SetVisibility(ESlateVisibility::Hidden);
		PauseMenuInstance->SetVisibility(ESlateVisibility::Visible);
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly UIGm;
		PlayerController->SetInputMode(UIGm);
		PlayerController->SetPause(true);
		PauseMenuInstance->SetOwningPlayer(PlayerController);
		PauseMenuInstance->ResumeBtn->SetFocus();
	}
	else
	{
		PlayerController->SetPause(false);
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly UIGm;
		PlayerController->SetInputMode(UIGm);
		InGameUiInstance->SetVisibility(ESlateVisibility::Visible);
		PauseMenuInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AInGameUIHandler::ToggleOptions()
{
	bShowOptions = !bShowOptions;
	OptionsInstance->LoadProgressData();
	if(bShowOptions)
	{
		OptionsInstance->SetVisibility(ESlateVisibility::Visible);
		OptionsInstance->PausedBackgroundBlur->SetVisibility(ESlateVisibility::Visible);
		PauseMenuInstance->SetVisibility(ESlateVisibility::Hidden);
	}
	if(!bShowOptions)
	{
		OptionsInstance->SetVisibility(ESlateVisibility::Hidden);
		OptionsInstance->PausedBackgroundBlur->SetVisibility(ESlateVisibility::Hidden);
		PauseMenuInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInGameUIHandler::BeginPlay()
{
	Super::BeginPlay();
	InGameUiInstance = CreateWidget<UInGameUIWidget>(GetWorld(), InGameUiClass);
	PauseMenuInstance = CreateWidget<UPauseWidget>(GetWorld(), PauseMenuClass);
	OptionsInstance = CreateWidget<UOptionsWidget>(GetWorld(), OptionsClass);

	if (InGameUiInstance)
	{
		InGameUiInstance->AddToViewport();
	}
	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport();
		PauseMenuInstance->SetVisibility(ESlateVisibility::Hidden);
		PauseMenuInstance->ResumeBtn->OnClicked.AddDynamic(this, &AInGameUIHandler::TogglePause);
	//	PauseMenuInstance->QuitBtn->OnClicked.AddDynamic(this, &AInGameUIHandler::P_PressQuit);
		PauseMenuInstance->OptionsBtn->OnClicked.AddDynamic(this, &AInGameUIHandler::ToggleOptions);
	}
	if(OptionsInstance)
	{
		OptionsInstance->AddToViewport();
		OptionsInstance->SetVisibility(ESlateVisibility::Hidden);
		OptionsInstance->BackBtn->OnClicked.AddDynamic(this, &AInGameUIHandler::ToggleOptions);
	}
}

void AInGameUIHandler::Tick(float DeltaTime)
{
	//in case we want a precise good looking timer.
	Super::Tick(DeltaTime);
	TotalTime += DeltaTime;
	Min = ((int)TotalTime / 60);
	Sec = ((int)TotalTime % 60);
	MSec = ((int)(TotalTime * 1000) % 1000);
	if (InGameUiInstance)
	{
		InGameUiInstance->SetTime(MSec, Sec, Min);
	}
}
