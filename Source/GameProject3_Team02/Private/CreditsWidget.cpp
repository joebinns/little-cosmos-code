// Fill out your copyright notice in the Description page of Project Settings.


#include "CreditsWidget.h"

void UCreditsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BackBtn = Cast<UButton>(GetWidgetFromName(TEXT("BackBtn")));
}
