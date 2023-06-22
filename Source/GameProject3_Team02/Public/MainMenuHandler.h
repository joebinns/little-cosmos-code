#pragma once
#include "CoreMinimal.h"
#include "CreditsWidget.h"
#include "LevelInfoWidget.h"
#include "LevelSelectionWidget.h"
#include "MainMenuWidget.h"
#include "OptionsWidget.h"
#include "GameFramework/Actor.h"
#include "MainMenuHandler.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API AMainMenuHandler : public AActor
{
	GENERATED_BODY()

public:
	AMainMenuHandler();

#pragma region REFS
	//MainMenu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UMainMenuWidget* MainMenuInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClasses")
	TSubclassOf<UMainMenuWidget> MainMenuClass;
	//Options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UOptionsWidget* OptionsInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClasses")
	TSubclassOf<UOptionsWidget> OptionsClass;
	//Credits
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UCreditsWidget* CreditsInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClasses")
	TSubclassOf<UCreditsWidget> CreditsClass;

	//data table for leaderboard (not used)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTables")
	UDataTable* RatingsDataTable;
#pragma endregion

#pragma region FUNCTIONS
	//ButtonPresses/Destruction
	UFUNCTION()
	void MM_PressPlayBtn();
	UFUNCTION()
	void MM_PressOptionsBtn();
	UFUNCTION()
	void MM_PressCreditsBtn();
	UFUNCTION()
	void DestroyWidgets();
	UFUNCTION()
	void O_OnPressBackBtn();
	UFUNCTION()
	void OnNarrativeCheckBoxToggled(bool bIsChecked);
	UFUNCTION()
	void C_OnPressBackBtn();

	
	//delayed functions
	UFUNCTION()
	void HideMainShowOptions();
	UFUNCTION()
	void ShowMainHideOptions();

#pragma endregion

#pragma region TIMERHANDLES
	FTimerHandle MainMenuWidgetDelay;
	FTimerHandle LevelSelectionWidgetDelay;
	FTimerHandle LevelInfoWidgetDelay;
	FTimerHandle OptionsWidgetsDelay;
#pragma endregion

protected:
	virtual void BeginPlay() override;
};
