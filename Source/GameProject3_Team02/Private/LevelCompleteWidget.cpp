// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelCompleteWidget.h"

#include "InGameUIHandler.h"
#include "Kismet/GameplayStatics.h"

void ULevelCompleteWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MainMenuBtn = Cast<UButton>(GetWidgetFromName(TEXT("MainMenuBtn")));
	NextLevelBtn = Cast<UButton>(GetWidgetFromName(TEXT("NextLevelBtn")));
	TotalTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TotalTimeText")));
	OverallRating = Cast<UTextBlock>(GetWidgetFromName(TEXT("OverallRating")));

	MainMenuBtn->OnClicked.AddDynamic(this, &ULevelCompleteWidget::OnMainMenuClick);
	NextLevelBtn->OnClicked.AddDynamic(this, &ULevelCompleteWidget::OnNextLevelClick);
	
	AInGameUIHandler* MainGameHandler = Cast<AInGameUIHandler>(UGameplayStatics::GetActorOfClass(GetWorld(), AInGameUIHandler::StaticClass()));
	TotalTimeText->SetText(FText::FromString("TIME: " + MainGameHandler->InGameUiInstance->FullTimeString));
}

void ULevelCompleteWidget::OnMainMenuClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu", false);
}

void ULevelCompleteWidget::OnNextLevelClick()
{
}
