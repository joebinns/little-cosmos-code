#pragma once

#include "CoreMinimal.h"
#include "PowerUpHandler.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UCLASS()
class GAMEPROJECT3_TEAM02_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerUp();

#pragma region FUNCTIONS
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
#pragma endregion

#pragma region REFS
	APowerUpHandler* PowerUpHandler;
#pragma endregion 

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
