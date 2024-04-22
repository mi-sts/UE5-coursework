// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponProjectileFactory.h"
#include "LaserWeaponProjectileFactory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API ULaserWeaponProjectileFactory : public UWeaponProjectileFactory
{
	GENERATED_BODY()

public:
	ULaserWeaponProjectileFactory();

protected:
	virtual void OnProjectileCreation(float Damage) override;

	UPROPERTY(EditAnywhere)
	float LaserHitDistance = 1000.0f;
};
