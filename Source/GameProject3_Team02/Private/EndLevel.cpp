#include "EndLevel.h"
#include "InGameUIHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

AEndLevel::AEndLevel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEndLevel::EndLevel()
{
	FString ContextString;
	LevelRatingRow = RatingsDataTable->FindRow<FLevelPostCompletionData>(
		FName(FString::FromInt(LevelIndex)), ContextString);
	LevelCompleteInstance = CreateWidget<ULevelCompleteWidget>(GetWorld(), LevelCompleteClass);
	if (LevelCompleteInstance != nullptr)
	{
		LevelCompleteInstance->AddToViewport();
	}
	LevelRatingRow->LevelRating = CalculateEndRating();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
}

FString AEndLevel::CalculateEndRating()
{
	FString Temp;
	AInGameUIHandler* MainGameHandler = Cast<AInGameUIHandler>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AInGameUIHandler::StaticClass()));

	if (UKismetStringLibrary::Conv_StringToInt(LevelRatingRow->RankingStrength) == 0)
	{
		LevelRatingRow->RankingStrength = "7";
	}

	int32 TargetRankingStrength;
	int32 RankingStrength = UKismetStringLibrary::Conv_StringToInt(LevelRatingRow->RankingStrength);


	if (MainGameHandler->TotalTime < SPlus)
	{
		Temp = "S+";
		LevelRatingRow->RankingStrength = "1";
		TargetRankingStrength = 1;
		LevelCompleteInstance->OverallRating->SetText(FText::FromString("Overall Rating: S+"));
	}
	if (MainGameHandler->TotalTime > SPlus && MainGameHandler->TotalTime < S)
	{
		Temp = "S";
		LevelRatingRow->RankingStrength = "2";
		TargetRankingStrength = 2;
		LevelCompleteInstance->OverallRating->SetText(FText::FromString("Overall Rating: S"));
	}
	if (MainGameHandler->TotalTime > S && MainGameHandler->TotalTime < A)
	{
		Temp = "A";
		LevelRatingRow->RankingStrength = "3";
		TargetRankingStrength = 3;
		LevelCompleteInstance->OverallRating->SetText(FText::FromString("Overall Rating: A"));
	}
	if (MainGameHandler->TotalTime > A && MainGameHandler->TotalTime < B)
	{
		Temp = "B";
		LevelRatingRow->RankingStrength = "4";
		TargetRankingStrength = 4;
		LevelCompleteInstance->OverallRating->SetText(FText::FromString("Overall Rating: B"));
	}
	if (MainGameHandler->TotalTime > B && MainGameHandler->TotalTime < C)
	{
		Temp = "C";
		LevelRatingRow->RankingStrength = "5";
		TargetRankingStrength = 5;
		LevelCompleteInstance->OverallRating->SetText(FText::FromString("Overall Rating: C"));
	}
	if (MainGameHandler->TotalTime > C)
	{
		Temp = "C-";
		LevelRatingRow->RankingStrength = "6";
		TargetRankingStrength = 6;
		LevelCompleteInstance->OverallRating->SetText(FText::FromString("Overall Rating: C-"));
	}
	if (TargetRankingStrength < RankingStrength)
	{
		return Temp;
	}
	else
	{
		return LevelRatingRow->LevelRating;
	}
}

void AEndLevel::BeginPlay()
{
	Super::BeginPlay();
}
