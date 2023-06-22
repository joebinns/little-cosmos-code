// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeBtn = Cast<UButton>(GetWidgetFromName(TEXT("ResumeBtn")));
	QuitBtn = Cast<UButton>(GetWidgetFromName(TEXT("QuitBtn")));
	OptionsBtn = Cast<UButton>(GetWidgetFromName(TEXT("OptionsBtn")));
}
