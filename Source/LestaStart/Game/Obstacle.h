// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class LESTASTART_API AObstacle : public AActor
{
	GENERATED_BODY()

public:
	AObstacle();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void OnHealthChanged(float CurrentHealth);

	UPROPERTY()
	UHealthComponent* HealthComponent;
};
