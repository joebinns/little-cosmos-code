#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "LevelCompleteWidget.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API ULevelCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
#pragma region BINDINGS
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* NextLevelBtn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalTimeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OverallRating;
#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION()
	void OnMainMenuClick();

	UFUNCTION()
	void OnNextLevelClick();
#pragma endregion
};
