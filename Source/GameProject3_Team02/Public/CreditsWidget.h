// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "CreditsWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT3_TEAM02_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* CreditsScroll;
};
