#include "MainMenuHandler.h"

#include "CreditsWidget.h"
#include "GP3_GameInstance.h"
#include "OptionsWidget.h"

AMainMenuHandler::AMainMenuHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuHandler::MM_PressPlayBtn()
{

}

void AMainMenuHandler::MM_PressOptionsBtn()
{
	OptionsInstance->LoadProgressData();
	OptionsInstance->SetVisibility(ESlateVisibility::Visible);
	HideMainShowOptions();
}

void AMainMenuHandler::MM_PressCreditsBtn()
{
	MainMenuInstance->SetVisibility(ESlateVisibility::Hidden);
	CreditsInstance->SetVisibility(ESlateVisibility::Visible);
}

void AMainMenuHandler::HideMainShowOptions()
{
	MainMenuInstance->SetVisibility(ESlateVisibility::Hidden);
	OptionsInstance->SetVisibility(ESlateVisibility::Visible);
}

void AMainMenuHandler::ShowMainHideOptions()
{
	OptionsInstance->SetVisibility(ESlateVisibility::Hidden);
	MainMenuInstance->SetVisibility(ESlateVisibility::Visible);
}

void AMainMenuHandler::DestroyWidgets()
{
	if (IsValid(MainMenuInstance))
	{
		MainMenuInstance->RemoveFromParent();
		MainMenuInstance->ConditionalBeginDestroy();
	}
	if (IsValid(OptionsInstance))
	{
		OptionsInstance->RemoveFromParent();
		OptionsInstance->ConditionalBeginDestroy();
	}
	if(IsValid(CreditsInstance))
	{
		CreditsInstance->RemoveFromParent();
		CreditsInstance->ConditionalBeginDestroy();
	}
}

void AMainMenuHandler::O_OnPressBackBtn()
{
	ShowMainHideOptions();
}

void AMainMenuHandler::OnNarrativeCheckBoxToggled(bool bIsChecked)
{
	UGP3_GameInstance* GameInstance = GetGameInstance<UGP3_GameInstance>();
	if (bIsChecked)
	{
		if (GameInstance)
		{
			GameInstance->bNarrativeModeEnabled = true;
		}
	}
	else
	{
		if (GameInstance)
		{
			GameInstance->bNarrativeModeEnabled = false;
		}
	}
}

void AMainMenuHandler::C_OnPressBackBtn()
{
	MainMenuInstance->SetVisibility(ESlateVisibility::Visible);
	CreditsInstance->SetVisibility(ESlateVisibility::Hidden);
}


void AMainMenuHandler::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;

	MainMenuInstance = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuClass);
	if (MainMenuInstance != nullptr)
	{
		MainMenuInstance->SetOwningPlayer(PlayerController);
		MainMenuInstance->PlayBtn->OnClicked.AddDynamic(this, &AMainMenuHandler::MM_PressPlayBtn);
		MainMenuInstance->OptionsBtn->OnClicked.AddDynamic(this, &AMainMenuHandler::MM_PressOptionsBtn);
		MainMenuInstance->CreditsBtn->OnClicked.AddDynamic(this, &AMainMenuHandler::MM_PressCreditsBtn);
		MainMenuInstance->SetOwningPlayer(PlayerController);
		MainMenuInstance->PlayBtn->SetFocus();
		MainMenuInstance->AddToViewport();
	}

	OptionsInstance = CreateWidget<UOptionsWidget>(GetWorld(), OptionsClass);
	if (OptionsInstance != nullptr)
	{
		OptionsInstance->BackBtn->OnClicked.AddDynamic(this, &AMainMenuHandler::O_OnPressBackBtn);
		OptionsInstance->AddToViewport();
		OptionsInstance->SetVisibility(ESlateVisibility::Hidden);
		OptionsInstance->SetOwningPlayer(PlayerController);
		
	}
	CreditsInstance = CreateWidget<UCreditsWidget>(GetWorld(), CreditsClass);
	if(CreditsInstance != nullptr)
	{
		CreditsInstance->SetOwningPlayer(PlayerController);
		CreditsInstance->BackBtn->OnClicked.AddDynamic(this, &AMainMenuHandler::C_OnPressBackBtn);
		CreditsInstance->AddToViewport();
		CreditsInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}
