// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorGravitySamplerComponent.h"
#include "GravityFieldComponent.h"
#include "Kismet/GameplayStatics.h"

void UEditorGravitySamplerComponent::UpdateGravityFields()
{
	GravityFields.Empty();
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (AActor* Actor : AllActors)
	{
		TArray<UGravityFieldComponent*> ActorsGravityFields;
		Actor->GetComponents<UGravityFieldComponent>(ActorsGravityFields);
		if (ActorsGravityFields.Num() > 0)
			GravityFields.Append(ActorsGravityFields);
	}
}
