// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/MathsUtilities.h"
#include "Kismet/KismetMathLibrary.h"

/*
 * Some examples of usage:
 * 
 * FRotator WorldRotation = FMathUtilities::CombineRotators(LocalRotationToConvert, RelativeObjectWorldRotation);
 * RelativeObjectWorldRotation is the world rotation of the object that the local rotation is relative to
 * 
 * FRotator LocalRotation = FMathUtilities::CombineRotators(WorldRotationToConvert, -RelativeObjectWorldRotation);
 * We use a negative version of RelativeObjectWorldRotation because we are removing that from our WorldRotationToConvert to get a local rotation
 */
FRotator FMathsUtilities::CombineRotators(FRotator RotatorA, FRotator RotatorB)
{
	const FQuat QuaternionA = FQuat(RotatorA);
	const FQuat QuaternionB = FQuat(RotatorB);
	return FRotator(QuaternionB * QuaternionA);
}

FVector FMathsUtilities::MoveTowards(FVector Current, FVector Target, float MaxDeltaDistance)
{
	FVector A = Target - Current;
	float Magnitude = A.Length();
	if (Magnitude <= MaxDeltaDistance || Magnitude == 0.f)
	{
		return Target;
	}
	return Current + A / Magnitude * MaxDeltaDistance;
}

// Adapted from source: https://answers.unity.com/questions/467614/what-is-the-source-code-of-quaternionlookrotation.html
FQuat FMathsUtilities::LookRotation(FVector Forward, FVector Upward)
{
	Forward = Forward.GetSafeNormal();

	FVector VectorA = Forward;
	FVector VectorB = FVector::CrossProduct(Upward, VectorA).GetSafeNormal();
	FVector VectorC = FVector::CrossProduct(VectorA, VectorB);
	
	float BX = VectorB.X; // 00
	float BY = VectorB.Y; // 01
	float BZ = VectorB.Z; // 02
	
	float CX = VectorC.X; // 10
	float CY = VectorC.Y; // 11
	float CZ = VectorC.Z; // 12
	
	float AX = VectorA.X; // 20
	float AY = VectorA.Y; // 21
	float AZ = VectorA.Z; // 22

	float Num8 = (BX + CY) + AZ;
	FQuat Quaternion;
	if (Num8 > 0.f)
	{
		float Num = UKismetMathLibrary::Sqrt(Num8 + 1.f);
		Quaternion.W = Num * 0.5f;
		Num = 0.5f / Num;
		Quaternion.X = (CZ - AY) * Num;
		Quaternion.Y = (AX - BZ) * Num;
		Quaternion.Z = (BY - CX) * Num;
		return Quaternion;
	}
	if ((BX >= CY) & (BX >= AZ))
	{
		float Num7 = UKismetMathLibrary::Sqrt(((1.f + BX) - CY) - AZ);
		float Num4 = 0.5f / Num7;
		Quaternion.X = 0.5f * Num7;
		Quaternion.Y = (BY + CX) * Num4;
		Quaternion.Z = (BZ + AX) * Num4;
		Quaternion.W = (CZ - AY) * Num4;
		return Quaternion;
	}
	if (CY > AZ)
	{
		float Num6 = UKismetMathLibrary::Sqrt(((1.f + CY) - BX) - AZ);
		float Num3 = 0.5f / Num6;
		Quaternion.X = (CX + BY) * Num3;
		Quaternion.Y = 0.5f * Num6;
		Quaternion.Z = (AY + CZ) * Num3;
		Quaternion.W = (AX - BZ) * Num3;
		return Quaternion;
	}
	float Num5 = UKismetMathLibrary::Sqrt(((1.f + AZ) - BX) - CY);
	float Num2 = 0.5f / Num5;
	Quaternion.X = (AX + BZ) * Num2;
	Quaternion.Y = (AY + CZ) * Num2;
	Quaternion.Z = 0.5f * Num5;
	Quaternion.W = (BY - CX) * Num2;
	return Quaternion;
}

FQuat FMathsUtilities::ShortestRotation(FQuat A, FQuat B)
{
	if (Dot(A, B) < 0.f)
	{
		return A * (-B).Inverse();
	}
	return A * B.Inverse();
}

float FMathsUtilities::Dot(FQuat A, FQuat B)
{
	return A.X * B.X + A.Y * B.Y + A.Z * B.Z + A.W * B.W;
}