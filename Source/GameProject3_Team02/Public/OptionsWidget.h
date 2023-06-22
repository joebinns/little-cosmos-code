#pragma once
#include "CoreMinimal.h"
#include "LevelPostCompletionData.h"
#include "Blueprint/UserWidget.h"
#include "Components/BackgroundBlur.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Engine/DataTable.h"
#include "OptionsWidget.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;


public:
#pragma region REFS
	FLevelPostCompletionData* DataRow;
#pragma endregion

#pragma region BINDINGS
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;

	UBackgroundBlur* PausedBackgroundBlur;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* CloseOptions;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* OpenOptions;
#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION()
	void ClearDataTableRow(UDataTable* DataTable, FName RowName);

	UFUNCTION(BlueprintImplementableEvent)
	void LoadProgressData();

#pragma endregion
};
