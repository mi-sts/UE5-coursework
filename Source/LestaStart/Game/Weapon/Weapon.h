// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponClipSystem.h"
#include "WeaponProjectileFactory.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class LESTASTART_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	
	void PullTrigger();
	
	void ReleaseTrigger();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category="QWE")
	TObjectPtr<UWeaponProjectileFactory> ProjectileFactory;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponClipSystem> WeaponClipSystem;
	
public:
	virtual void Tick(float DeltaTime) override;
};
