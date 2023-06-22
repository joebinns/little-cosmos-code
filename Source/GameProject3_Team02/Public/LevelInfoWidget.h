#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "LevelInfoWidget.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API ULevelInfoWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

public:
#pragma region BINDINGS
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelInfoText;

	UPROPERTY(Transient, meta = ( BindWidgetAnim ))
	UWidgetAnimation* OpenLevelInfo;
#pragma endregion
};
