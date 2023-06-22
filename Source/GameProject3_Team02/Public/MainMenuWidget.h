#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
#pragma region BINDINGS
	UPROPERTY(meta = (BindWidget))
	UButton* PlayBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsBtn;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreditsBtn;
#pragma endregion
};
