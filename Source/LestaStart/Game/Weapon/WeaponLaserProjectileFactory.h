// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponProjectileFactory.h"
#include "WeaponLaserProjectileFactory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API UWeaponLaserProjectileFactory : public UWeaponProjectileFactory
{
	GENERATED_BODY()

public:
	UWeaponLaserProjectileFactory();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void CreateProjectile() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float LaserHitDistance = 1000.0f;
};
