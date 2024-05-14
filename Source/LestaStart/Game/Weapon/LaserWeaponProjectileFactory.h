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
	UPROPERTY(EditAnywhere)
	float LaserHitDistance = 1000.0f;

	virtual void OnServerProjectileCreation(float Damage) override;
	virtual void CreateProjectileView(float Damage) override;
	
	bool GetLaserTraceHitResult(FHitResult& HitResult);
	UFUNCTION(Server, Reliable)
	void ServerCreateDamageTrace(float Damage);
	UFUNCTION(Server, Reliable)
	void ServerApplyTraceDamage(AActor* ToActor, float Damage, FDamageEvent DamageEvent, AActor* DamageCauser);

	FVector GetLaserTraceStartLocation();
	FVector GetLaserTraceStartToEndVector();
};
