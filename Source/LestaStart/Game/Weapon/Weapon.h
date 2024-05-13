// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponAmmoSystem.h"
#include "WeaponProjectileFactory.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS(Abstract)
class LESTASTART_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	
	virtual void PullTrigger();
	virtual void ReleaseTrigger();
	virtual void Activate(UCameraComponent* CameraComponent);
	virtual void Deactivate();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Replicated)
	TObjectPtr<UWeaponProjectileFactory> ProjectileFactory;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponAmmoSystem> WeaponAmmoSystem;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsTriggered;
	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsVisible;
	
	void TakeShot(float Damage);
	void SetVisible(bool Visible);
	void InitializeMesh(const FString& MeshReferenceName);
	FTransform GetMuzzleTransform();
	FTransform GetCameraTransform();

	UPROPERTY(Replicated)
	UCameraComponent* PlayerCameraComponent;
	UPROPERTY(Replicated)
	USkeletalMeshSocket* MuzzleSocket;
	
public:
	virtual void Tick(float DeltaTime) override;
};
