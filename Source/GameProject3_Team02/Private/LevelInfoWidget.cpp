#include "LevelInfoWidget.h"

void ULevelInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LevelNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelNameText")));
	LevelInfoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("LevelInfoText")));
}
