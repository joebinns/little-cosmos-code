#pragma once

#include "CoreMinimal.h"
#include "InGameUIHandler.h"
#include "LaunchPad.h"
#include "GameFramework/Actor.h"
#include "PowerUpHandler.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API APowerUpHandler : public AActor
{
	GENERATED_BODY()

public:
	APowerUpHandler();
	void BeginPlay();

#pragma region DESIGNER-DESIRABLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PowerUpSettings")
	int32 PowerUpsRequired;
#pragma endregion

#pragma region FUNCTIONS
	void CollectPowerUp();
	void ResetObjective();
#pragma endregion

#pragma region REFS
	AInGameUIHandler* InGameUIHandler;
	TArray<AActor*> LaunchPads;
	APlayerPhysicsActor* PlayerActor;
	ALaunchPad* ClosestLaunchPad = nullptr;
#pragma endregion

#pragma region VAR
	int32 CollectedPowerUps;
	int32 LaunchPadIndex = 1;
#pragma endregion
	
	virtual void Tick(float DeltaTime) override;
};
