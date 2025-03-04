﻿
#pragma once

#include "CoreMinimal.h"
#include "WeaponProjectileFactory.h"
#include "SphereWeaponProjectileFactory.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LESTASTART_API USphereWeaponProjectileFactory : public UWeaponProjectileFactory
{
	GENERATED_BODY()

public:
	USphereWeaponProjectileFactory();

	void Initialize(float SphereDamageRadius);
	
	virtual void CreateProjectileView(float Damage) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnServerProjectileCreation(float Damage) override;

	UFUNCTION(Server, Reliable)
	void ServerApplyRadialDamage(float Damage);

	float DamageRadius;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
