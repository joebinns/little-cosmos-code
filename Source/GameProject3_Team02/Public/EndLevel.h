#pragma once

#include "CoreMinimal.h"
#include "LevelCompleteWidget.h"
#include "LevelPostCompletionData.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "EndLevel.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API AEndLevel : public AActor
{
	GENERATED_BODY()
	
public:
	AEndLevel();

#pragma region DESIGNER-DESIRABLE
	UPROPERTY(EditAnywhere, Category="LevelIndex")
	int32 LevelIndex;
	UPROPERTY(EditAnywhere, Category="TimeRequiredForRating")
	int32 SPlus;
	UPROPERTY(EditAnywhere, Category="TimeRequiredForRating")
	int32 S;
	UPROPERTY(EditAnywhere, Category="TimeRequiredForRating")
	int32 A;
	UPROPERTY(EditAnywhere, Category="TimeRequiredForRating")
	int32 B;
	UPROPERTY(EditAnywhere, Category="TimeRequiredForRating")
	int32 C;
#pragma endregion 

#pragma region REFS
	UPROPERTY()
	ULevelCompleteWidget* LevelCompleteInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetClasses")
	TSubclassOf<ULevelCompleteWidget> LevelCompleteClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DataTable")
	UDataTable* RatingsDataTable;
	FLevelPostCompletionData* LevelRatingRow;
#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void EndLevel();
	UFUNCTION()
	FString CalculateEndRating();
#pragma endregion
	
protected:
	virtual void BeginPlay() override;
};
