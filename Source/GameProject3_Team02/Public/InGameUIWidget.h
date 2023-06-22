#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/TimelineComponent.h"
#include "Components/VerticalBox.h"
#include "InGameUIWidget.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API UInGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
public:
#pragma region BINDINGS
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TakeoffCountdown;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* MegaLaunchPadCountdownPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* HudPanel;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ObjectiveText;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Objective;

	UFUNCTION(BlueprintImplementableEvent)
	void CallEndLevel();
	
	bool bIsAtTargetValue = false;

	float CurrentPercent;

	int CurrentProgress;

	float targetValue;

	FTimerHandle LerpTimerHandle;

	float CurrentAlpha = 0;

	UPROPERTY()
	UTimelineComponent* ProgressTimeline;

	UPROPERTY()
	FString FullTimeString;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION()
	void SetTime(int MSec, int Sec, int Min);
	void IncreaseProgress();
	void LerpProgress(float f);
	void ResetProgress();
#pragma endregion
};
