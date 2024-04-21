// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "DiscreteWeapon.generated.h"

UCLASS()
class LESTASTART_API ADiscreteWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ADiscreteWeapon();

	virtual void PullTrigger() override;
	virtual void ReleaseTrigger() override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float ShotsDelay;
	UPROPERTY(EditAnywhere)
	float ShotDamage;
	
	FTimerHandle ShotTimerHandle;
	FTimerDelegate ShotFuncDelegate;

public:
	virtual void Tick(float DeltaTime) override;
};
