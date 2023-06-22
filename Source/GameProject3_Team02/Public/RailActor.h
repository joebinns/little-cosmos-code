#pragma once

#include "CoreMinimal.h"
#include "PlayerPawn.h"
#include "PlayerPhysicsActor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/PhysicsCollisionHandler.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RailActor.generated.h"

enum class ERailState: uint8
{
	RAILSTATE_STARTRAILING UMETA(DisplayName="BeginRail"),
	RAILSTATE_EXITRAILING UMETA(DisplayName="ExitRail"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginRailSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitRailSignature);

UCLASS()
class GAMEPROJECT3_TEAM02_API ARailActor : public AActor
{
	GENERATED_BODY()
	
public:
	ARailActor();
	~ARailActor();

	void SetRailState(ERailState State);

	UPROPERTY(BlueprintAssignable)
	FOnBeginRailSignature OnBeginRailDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnExitRailSignature OnExitRailDelegate;

#pragma region DESIGNER-DESIRABLE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RailSettings")
	float RailSpeed = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RailSettings")
	float RailDetachImpulseMagnitude = 100000.0f;
#pragma endregion

#pragma region FUNCTIONS
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	void OnConstruction(const FTransform& Transform);
	void MoveMeshAlongSpline(UCapsuleComponent* ColliderComponentToMove);

	UFUNCTION()
	void DetachFromRail();

	UFUNCTION()
	void DisableCollisionForAllMeshes();

	UFUNCTION()
	void EnableCollisionForAllMeshes();
	
	void StartRailBoost();
	void EndRailBoost();
	void SetupSpline();
	APlayerPawn* GetPlayerPawn();

#pragma endregion

#pragma region BOOLS
	bool bRailingStarted;
	bool bRailingBackwards;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerIsOnRail = false;
	
	bool bPlayerIsBoosting = false;
#pragma endregion

#pragma region SPLINE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SplineMeshList")
	TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(BlueprintReadWrite, Category="Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline")
	UStaticMesh* StartMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline")
	UStaticMesh* EndMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	int32 CurrentPointIndex;
	float Distance;
	float CurrentTime;
	FVector SplineTangent;
#pragma endregion

#pragma region REFS
	APlayerPhysicsActor* PlayerActor;
	UCapsuleComponent* PlayerRef;
#pragma endregion

	FTimerHandle AutoDetachDelay;
	FTimerHandle DetachDelay;
	FTimerHandle StopBoostingDelay;

	ERailState RailState;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
