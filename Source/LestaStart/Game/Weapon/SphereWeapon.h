// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "SphereWeapon.generated.h"

UCLASS()
class LESTASTART_API ASphereWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ASphereWeapon();

	virtual void PullTrigger() override;
	virtual void ReleaseTrigger() override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxChargeValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargePerSecondIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageRadius;
	UPROPERTY(VisibleAnywhere)
	float CurrentChargeValue;

public:
	virtual void Tick(float DeltaTime) override;
};
