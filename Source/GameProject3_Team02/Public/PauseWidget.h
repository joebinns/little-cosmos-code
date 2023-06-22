#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseWidget.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UButton* ResumeBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn;

	UPROPERTY(meta = (BindWidget))
	UButton* OptionsBtn;
};
