// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "LaserWeapon.generated.h"

UCLASS()
class LESTASTART_API ALaserWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ALaserWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float DamagePerSecond;

public:
	virtual void Tick(float DeltaTime) override;
};
