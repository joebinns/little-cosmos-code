#include "LevelSelectionWidget.h"

#include "GP3_GameInstance.h"
#include "LevelPostCompletionData.h"

void ULevelSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//buttons
	BackBtn = Cast<UButton>(GetWidgetFromName(TEXT("BackBtn")));
	Level1Btn = Cast<UButton>(GetWidgetFromName(TEXT("Level1Btn")));
	Level2Btn = Cast<UButton>(GetWidgetFromName(TEXT("Level2Btn")));
	Level3Btn = Cast<UButton>(GetWidgetFromName(TEXT("Level3Btn")));
	Level4Btn = Cast<UButton>(GetWidgetFromName(TEXT("Level4Btn")));
	Level5Btn = Cast<UButton>(GetWidgetFromName(TEXT("Level5Btn")));


	//scuffed text
	RatingLevel1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("RatingLevel1")));
	RatingLevel2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("RatingLevel2")));
	RatingLevel3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("RatingLevel3")));
	RatingLevel4 = Cast<UTextBlock>(GetWidgetFromName(TEXT("RatingLevel4")));
	RatingLevel5 = Cast<UTextBlock>(GetWidgetFromName(TEXT("RatingLevel5")));


	LevelRatings.Add(RatingLevel1);
	LevelRatings.Add(RatingLevel2);
	LevelRatings.Add(RatingLevel3);
	LevelRatings.Add(RatingLevel4);
	LevelRatings.Add(RatingLevel5);


	RefreshLevelData();
}

void ULevelSelectionWidget::RefreshLevelData()
{
	FString ContextString;

	for (int i = 1; i <= 5; i++)
	{
		UGP3_GameInstance* GameInstance = GetGameInstance<UGP3_GameInstance>();
		if (GameInstance)
			if (!GameInstance->bNarrativeModeEnabled)
			{
				FName Temp = FName(FString::FromInt(i));
				LevelRatingRow = RatingsDataTable->FindRow<FLevelPostCompletionData>(Temp, ContextString);
				if (LevelRatingRow)
				{
					FString LevelRating = LevelRatingRow->LevelRating;
					LevelRatings[i - 1]->SetText(FText::FromString(LevelRating));
				}
			}
			else
			{
				LevelRatings[i - 1]->SetText(FText::FromString(""));
			}
	}
}
