// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class SPACETIMEGAME_API FPhysicsUtilities
{
#pragma region SUBSTEP_TICK
	public:
		static FBodyInstance* GetBodyInstance(const USceneComponent* SceneComponent);
		static FBodyInstance* GetParentBodyInstance(const USceneComponent* SceneComponent);
		static FTransform GetSubstepComponentTransform(const USceneComponent* SceneComponent, FBodyInstance* ParentBodyInstance);
	
	protected:
		static void GetBoneTransformRecursive(FTransform& Transform, const USkeletalMeshComponent* SkeletalMeshComponent, const FName& Bone);
		static FTransform GetBoneRefTransformInComponentSpace(const USkeletalMeshComponent* SkeletalMeshComponent, const FName& Bone);
#pragma endregion
};
