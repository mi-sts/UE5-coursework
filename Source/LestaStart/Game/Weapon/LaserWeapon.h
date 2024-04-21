﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DiscreteWeapon.h"
#include "Weapon.h"
#include "LaserWeapon.generated.h"

UCLASS()
class LESTASTART_API ALaserWeapon : public ADiscreteWeapon
{
	GENERATED_BODY()

public:
	ALaserWeapon();

	virtual void Activate(UCameraComponent* CameraComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
