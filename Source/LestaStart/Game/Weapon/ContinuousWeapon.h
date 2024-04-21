// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ContinuousWeapon.generated.h"

UCLASS()
class LESTASTART_API AContinuousWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AContinuousWeapon();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	float DamagePerSecond;

public:
	virtual void Tick(float DeltaTime) override;
};
