#include "RailActor.h"
#include "GravityComponent.h"
#include "PlayerPawn.h"
#include "Components/SplineMeshComponent.h"

ARailActor::ARailActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if (SplineComponent)
	{
		bPlayerIsOnRail = true;
	}
}

ARailActor::~ARailActor()
{
	SplineMeshes.Empty();
}

void ARailActor::SetRailState(ERailState State)
{
	RailState = State;
	switch (RailState)
	{
	case ERailState::RAILSTATE_STARTRAILING:
		OnBeginRailDelegate.Broadcast();
		break;
	case ERailState::RAILSTATE_EXITRAILING:
		OnExitRailDelegate.Broadcast();
	}
}

void ARailActor::DisableCollisionForAllMeshes()
{
	UE_LOG(LogTemp, Warning, TEXT("DISABLING ALL COLLISION FOR RAIL"));
	for (USplineMeshComponent* MeshInList : SplineMeshes)
	{
		if (MeshInList)
		{
			MeshInList->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void ARailActor::EnableCollisionForAllMeshes()
{
	UE_LOG(LogTemp, Warning, TEXT("ENABLING ALL COLLISION FOR RAIL"));
	for (USplineMeshComponent* MeshInList : SplineMeshes)
	{
		if (MeshInList)
		{
			MeshInList->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ARailActor::StartRailBoost()
{
	if (!bPlayerIsBoosting)
	{
		bPlayerIsBoosting = true;
		RailSpeed += 250;
		GetWorldTimerManager().SetTimer(StopBoostingDelay, this, &ARailActor::EndRailBoost, 0.5f, false);
	}
}

void ARailActor::EndRailBoost()
{
	RailSpeed -= 250;
	bPlayerIsBoosting = false;
}

void ARailActor::SetupSpline()
{
	if (Mesh)
	{
		for (int SplineCount = 0; SplineCount < (SplineComponent->GetNumberOfSplinePoints() - 1); SplineCount++)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(
				this, USplineMeshComponent::StaticClass());
			SplineMesh->SetStaticMesh(Mesh);
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			SplineMesh->RegisterComponentWithWorld(GetWorld());
			SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

			const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(
				SplineCount, ESplineCoordinateSpace::Local);
			const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(
				SplineCount, ESplineCoordinateSpace::Local);
			const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(
				SplineCount + 1, ESplineCoordinateSpace::Local);
			const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(
				SplineCount + 1, ESplineCoordinateSpace::Local);

			SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
			SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SplineMesh->SetForwardAxis(ForwardAxis);

			if (StartMesh && SplineCount == 0)
			{
				SplineMesh->SetStaticMesh(StartMesh);
			}
			else if (EndMesh && SplineComponent->GetNumberOfSplinePoints() > 2 && SplineCount == (SplineComponent->
				GetNumberOfSplinePoints() - 2))
			{
				SplineMesh->SetStaticMesh(EndMesh);
			}
			else
			{
				SplineMesh->SetStaticMesh(Mesh);
			}
		}
	}
}

APlayerPawn* ARailActor::GetPlayerPawn()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PlayerController->GetPawn());
	return PlayerPawn;
}

void ARailActor::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerPhysicsActor::StaticClass()))
	{
		DisableCollisionForAllMeshes();
		PlayerActor = Cast<APlayerPhysicsActor>(OtherActor);
		if (PlayerActor)
		{
			PlayerActor->GetGravity()->Enabled = false;
			UCapsuleComponent* PlayerMesh = Cast<UCapsuleComponent>(PlayerActor->GetRootComponent());
			if (PlayerMesh)
			{
				GetPlayerPawn()->PhysicsActor->OnJumpDelegate.AddDynamic(this, &ARailActor::DetachFromRail);
				GetPlayerPawn()->UnbindMoveInput();
				GetPlayerPawn()->PhysicsActor->bIsOnRail = true;
				GetPlayerPawn()->PhysicsActor->SetJumpState(EJumpState::JE_None);
				PlayerRef = PlayerMesh;
				bPlayerIsOnRail = true;
				SetRailState(ERailState::RAILSTATE_STARTRAILING);
			}
		}
	}
}

void ARailActor::BeginPlay()
{
	Super::BeginPlay();
	SetupSpline();
	for (USceneComponent* ChildComponent : SplineComponent->GetAttachChildren())
	{
		USplineMeshComponent* SplineMesh = Cast<USplineMeshComponent>(ChildComponent);
		if (SplineMesh)
		{
			SplineMeshes.Add(SplineMesh);
			FString MeshName = SplineMesh->GetName();
		}
	}

	if (!SplineMeshes.IsEmpty())
	{
		for (USplineMeshComponent* MeshInList : SplineMeshes)
		{
			if (MeshInList)
			{
				MeshInList->SetEnableGravity(false);
				MeshInList->SetNotifyRigidBodyCollision(true);
				MeshInList->GetBodyInstance()->bLockYTranslation = true;
				MeshInList->GetBodyInstance()->bLockZTranslation = true;
				MeshInList->GetBodyInstance()->bLockXTranslation = true;
				MeshInList->GetBodyInstance()->bLockXRotation = true;
				MeshInList->GetBodyInstance()->bLockZRotation = true;
				MeshInList->GetBodyInstance()->bLockYRotation = true;
				MeshInList->GetBodyInstance()->SetDOFLock(EDOFMode::SixDOF);
				MeshInList->OnComponentHit.AddDynamic(this, &ARailActor::OnComponentHit);
				MeshInList->SetGenerateOverlapEvents(true);
				MeshInList->SetCollisionProfileName("IgnoreGroundCheck", true);
			}
		}
	}
	EnableCollisionForAllMeshes();
}

void ARailActor::OnConstruction(const FTransform& Transform)
{
	SetupSpline();
}

void ARailActor::MoveMeshAlongSpline(UCapsuleComponent* ColliderComponentToMove)
{
	if (SplineComponent && ColliderComponentToMove)
	{
		const float SplineLength = SplineComponent->GetSplineLength();
		const int32 NumSplinePoints = SplineComponent->GetNumberOfSplinePoints();
		float DistanceIncrement = RailSpeed * GetWorld()->DeltaTimeSeconds;

		FVector MeshForwardVector = ColliderComponentToMove->GetForwardVector();
		SplineTangent = SplineComponent->GetTangentAtDistanceAlongSpline(
			Distance, ESplineCoordinateSpace::World);

		if (!bRailingStarted)
		{
			FVector MeshLocation = ColliderComponentToMove->GetComponentLocation();
			float ClosestDistance = TNumericLimits<float>::Max();
			int32 ClosestPointIndex = 0;
			float Angle =
				FMath::Acos(FVector::DotProduct(MeshForwardVector.GetSafeNormal(), SplineTangent.GetSafeNormal()));
			Angle = FMath::RadiansToDegrees(Angle);
			if (Angle > 90.0f)
			{
				CurrentPointIndex = NumSplinePoints - 1;
				Distance = SplineLength - Distance;
				DistanceIncrement = -DistanceIncrement;
				bRailingBackwards = true;
			}
			else
			{
				bRailingBackwards = false;
			}

			for (int32 i = 0; i < NumSplinePoints; i++)
			{
				FVector SplinePointLocation = SplineComponent->GetLocationAtSplinePoint(
					i, ESplineCoordinateSpace::World);
				float DistanceToSpline = FVector::DistSquared(MeshLocation, SplinePointLocation);

				if (DistanceToSpline < ClosestDistance)
				{
					ClosestDistance = DistanceToSpline;
					ClosestPointIndex = i;
				}
			}

			CurrentPointIndex = ClosestPointIndex;
			Distance = SplineComponent->GetDistanceAlongSplineAtSplinePoint(ClosestPointIndex);
			bRailingStarted = true;
		}

		if (!bRailingBackwards)
		{
			if (Distance >= SplineLength || CurrentPointIndex >= NumSplinePoints)
			{
				DetachFromRail();

				UCapsuleComponent* PlayerRoot = Cast<UCapsuleComponent>(PlayerActor->GetRootComponent());
				FVector ImpulseVector = PlayerRef->GetForwardVector() * RailDetachImpulseMagnitude;
				if (PlayerRoot)
				{
					PlayerRoot->AddImpulse(ImpulseVector);
				}
				GetPlayerPawn()->BindMoveInput();
				return;
			}
			else if (CurrentTime >= 1.0f && CurrentPointIndex < NumSplinePoints - 1)
			{
				CurrentPointIndex++;
				CurrentTime = 0.0f;
			}
		}
		else
		{
			if (Distance <= 0 || CurrentPointIndex <= 0 - 1)
			{
				DetachFromRail();

				UCapsuleComponent* PlayerRoot = Cast<UCapsuleComponent>(PlayerActor->GetRootComponent());
				FVector ImpulseVector = PlayerRef->GetForwardVector() * RailDetachImpulseMagnitude;
				if (PlayerRoot)
				{
					PlayerRoot->AddImpulse(ImpulseVector);
				}

				GetPlayerPawn()->BindMoveInput();
				return;
			}
			else if (CurrentTime >= 1.0f && CurrentPointIndex > 0)
			{
				CurrentPointIndex--;
				CurrentTime = 0.0f;
			}
		}

		FVector TargetLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
			Distance, ESplineCoordinateSpace::World);
		FVector Offset = FVector(0.0f, 0.0f, 140.0f);

		FVector GravityDirection = PlayerActor->GetGravity()->SampleForceDirection();
		FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(FVector::DownVector, GravityDirection);
		FVector GravitySpaceOffset = DeltaGravityRotation.RotateVector(Offset);
		TargetLocation += GravitySpaceOffset;

		GetPlayerPawn()->PhysicsActor->FacingGoal = SplineTangent.GetSafeNormal() * (bRailingBackwards ? -1.f : 1.f);

		FVector CurrentLocation = ColliderComponentToMove->GetComponentLocation();
		FVector NewLocation =
			FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->DeltaTimeSeconds, 10.0f);

		ColliderComponentToMove->SetWorldLocation(NewLocation);

		if (!bRailingBackwards)
		{
			Distance += DistanceIncrement;
		}
		else
		{
			Distance -= DistanceIncrement;
		}
		CurrentTime += GetWorld()->DeltaTimeSeconds;
	}
}

void ARailActor::DetachFromRail()
{
	SetRailState(ERailState::RAILSTATE_EXITRAILING);
	GetPlayerPawn()->PhysicsActor->bIsOnRail = false;
	bPlayerIsOnRail = false;
	bRailingStarted = false;
	GetWorldTimerManager().SetTimer(DetachDelay, this, &ARailActor::EnableCollisionForAllMeshes, 0.3f, false);
	CurrentPointIndex = 0;
	PlayerActor->GetGravity()->Enabled = true;

	UCapsuleComponent* PlayerRoot = Cast<UCapsuleComponent>(PlayerActor->GetRootComponent());
	FVector ImpulseVector = PlayerRef->GetForwardVector() * RailDetachImpulseMagnitude;
	if (PlayerRoot)
	{
		PlayerRoot->AddImpulse(ImpulseVector);
	}
	GetPlayerPawn()->BindMoveInput();
	GetPlayerPawn()->PhysicsActor->OnJumpDelegate.RemoveDynamic(this, &ARailActor::DetachFromRail);
}

void ARailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bPlayerIsOnRail)
	{
		if (PlayerRef)
		{
			MoveMeshAlongSpline(PlayerRef);
		}
	}
}
