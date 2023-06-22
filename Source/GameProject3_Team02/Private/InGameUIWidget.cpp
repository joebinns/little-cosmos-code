#include "InGameUIWidget.h"
#include "GP3_GameInstance.h"
#include "Components/ProgressBar.h"

void UInGameUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TakeoffCountdown = Cast<UTextBlock>(GetWidgetFromName(TEXT("TakeoffCountdown")));
	MegaLaunchPadCountdownPanel = Cast<UVerticalBox>(GetWidgetFromName(TEXT("MegaLaunchPadCountdownPanel")));
	HudPanel = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("HudPanel")));
	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar")));
	ObjectiveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ObjectiveText")));
	Objective = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("Objective")));
}

void UInGameUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	LerpProgress(targetValue);
}

void UInGameUIWidget::SetTime(int MSec, int Sec, int Min)
{
	FString sMin, sSec, sMSec;

	if (Min < 10)
	{
		sMin = "0" + FString::FromInt(Min);
	}
	else
	{
		sMin = FString::FromInt(Min);
	}

	if (Sec < 10)
	{
		sSec = "0" + FString::FromInt(Sec);
	}
	else
	{
		sSec = FString::FromInt(Sec);
	}

	if (MSec < 10)
	{
		sMSec = "00" + FString::FromInt(MSec);
	}
	else
	{
		sMSec = FString::FromInt(MSec);
	}

	FullTimeString = sMin + ":" + sSec + ":" + sMSec;

	UGP3_GameInstance* GameInstance = GetGameInstance<UGP3_GameInstance>();
}

void UInGameUIWidget::IncreaseProgress()
{
	CurrentProgress++;
	switch (CurrentProgress)
	{
	case 1:
		targetValue = 0.136f;
		break;
	case 2:
		targetValue = 0.256f;
		break;
	case 3:
		targetValue = 0.376f;
		break;
	case 4:
		targetValue = 0.504f;
		break;
	case 5:
		targetValue = 0.624f;
		break;
	case 6:
		targetValue = 0.752f;
		break;
	case 7:
		targetValue = 0.872f;
		break;
	case 8:
		targetValue = 1.0f;
		break;
	}
}

void UInGameUIWidget::LerpProgress(float f)
{
	float CurrentValue = ProgressBar->Percent;
	float LerpedValue = FMath::Lerp(CurrentValue, f, 0.01f);
	ProgressBar->SetPercent(LerpedValue);

	if (FMath::IsNearlyEqual(CurrentValue, f))
	{
		ProgressBar->SetPercent(f);
	}
}

void UInGameUIWidget::ResetProgress()
{
	CurrentProgress = 0;
	targetValue = 0;
}
