// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
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

	UPROPERTY(EditAnywhere)
	float VisibilityDistance;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;
	UPROPERTY(VisibleAnywhere)
	ATurret* PossessedTurret;
	
	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	FRotator CalculateLookAtRotator(APawn* OtherPawn);

	ATurret* ControlledTurret;
	APawn* LastSeenPawn;
	bool IsFollowing;
	FTimerHandle StopFollowingTimerHandle;

public:
	virtual void Tick(float DeltaTime) override;
};
