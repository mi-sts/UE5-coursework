// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAmmoSystem.h"
#include "WeaponProjectileFactory.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS(Abstract)
class LESTASTART_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	
	void PullTrigger();
	
	void ReleaseTrigger();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponProjectileFactory> ProjectileFactory;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponAmmoSystem> WeaponAmmoSystem;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere)
	bool IsTriggered;

	void InitializeMesh(const FString& MeshReferenceName);
	void TakeShot();

	FTimerHandle ShotTimerHandle;
public:
	virtual void Tick(float DeltaTime) override;
};
