// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsWidget.h"
#include "UObject/PropertyAccessUtil.h"

void UOptionsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BackBtn = Cast<UButton>(GetWidgetFromName(TEXT("BackBtn")));
	PausedBackgroundBlur = Cast<UBackgroundBlur>(GetWidgetFromName(TEXT("PausedBackgroundBlur")));
}

void UOptionsWidget::ClearDataTableRow(UDataTable* DataTable, FName RowName)
{
	FString ContextString;
	if (DataTable)
	{
		DataRow = DataTable->FindRow<FLevelPostCompletionData>(RowName, ContextString);
		if (DataRow)
		{
			DataRow->LevelRating = nullptr;
			DataRow->RankingStrength = nullptr;
			DataTable->Modify();
		}
	}


	// const UScriptStruct* RowStruct = DataTable->GetRowStruct();
	// FTableRowBase* Row = DataTable->FindRow<FTableRowBase>(RowName, FString());
	//
	// if(Row)
	// {
	// 	for(TFieldIterator<FProperty> It(const_cast<UScriptStruct*>(RowStruct)); It; ++It)
	// 	{
	// 		FProperty* Property = *It;
	// 		Property->InitializeValue(Property->ContainerPtrToValuePtr<void>(Row));
	// 	}
	// 	DataTable->Modify();
	// }
}
