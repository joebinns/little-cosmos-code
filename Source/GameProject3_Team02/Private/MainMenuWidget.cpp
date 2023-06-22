#include "MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayBtn = Cast<UButton>(GetWidgetFromName(TEXT("PlayBtn")));
	OptionsBtn = Cast<UButton>(GetWidgetFromName(TEXT("OptionsBtn")));
	CreditsBtn = Cast<UButton>(GetWidgetFromName(TEXT("CreditsBtn")));
}
