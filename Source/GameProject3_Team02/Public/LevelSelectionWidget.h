#pragma once
#include "CoreMinimal.h"
#include "LevelPostCompletionData.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "LevelSelectionWidget.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API ULevelSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
#pragma region REFS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTable")
	UDataTable* RatingsDataTable;

	FLevelPostCompletionData* LevelRatingRow;
	TArray<UTextBlock*> LevelRatings;
#pragma endregion

#pragma region BINDINGS
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* Level1Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Level2Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Level3Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Level4Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Level5Btn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RatingLevel1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RatingLevel2;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RatingLevel3;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RatingLevel4;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RatingLevel5;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* CloseLevelSelection;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* OpenLevelSelection;
#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION()
	void RefreshLevelData();
#pragma endregion
};
