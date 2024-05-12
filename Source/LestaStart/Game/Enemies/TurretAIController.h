// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Turret.h"
#include "TurretAIController.generated.h"

UCLASS()
class LESTASTART_API ATurretAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATurretAIController();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Destroyed() override;

	void StopFollowing();
	void UpdateVisibleActors();

	UPROPERTY(EditAnywhere)
	float DetectingInterval;
	UPROPERTY(EditAnywhere)
	float VisibilityDistance;
	UPROPERTY(VisibleAnywhere)
	ATurret* PossessedTurret;
	
	UFUNCTION()
	void OnDetectActor(AActor* OtherActor);
	
	ATurret* ControlledTurret;
	AActor* LastSeenActor;
	bool IsFollowing;
	FTimerHandle StopFollowingTimerHandle;
	FTimerHandle DetectionTimerHandle;
	TArray<AActor*> VisibleActors;

public:
	virtual void Tick(float DeltaTime) override;
};
