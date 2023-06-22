// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameUIWidget.h"
#include "OptionsWidget.h"
#include "PauseWidget.h"
#include "GameFramework/Actor.h"
#include "InGameUIHandler.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API AInGameUIHandler : public AActor
{
	GENERATED_BODY()

public:
	AInGameUIHandler();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInGameUIWidget* InGameUiInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClasses")
	TSubclassOf<UInGameUIWidget> InGameUiClass;

	UPROPERTY()
	UPauseWidget* PauseMenuInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClasses")
	TSubclassOf<UPauseWidget> PauseMenuClass;

	UPROPERTY()
	UOptionsWidget* OptionsInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClasses")
	TSubclassOf<UOptionsWidget> OptionsClass;

	UFUNCTION()
	void P_PressQuit();
	
	UFUNCTION()
	void TogglePause();

	UFUNCTION()
	void ToggleOptions();
	
	bool bPaused;
	bool bShowOptions;

	float TotalTime;

	int Min;
	int Sec;
	int MSec;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
