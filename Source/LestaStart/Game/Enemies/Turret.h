// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class UWeaponProjectileFactory;

UCLASS()
class LESTASTART_API ATurret : public APawn
{
	GENERATED_BODY()

public:
	ATurret();
	
	UFUNCTION(BlueprintCallable)
	void RotateTo(FRotator Rotation);
	UFUNCTION(BlueprintCallable)
	void AssignProjectileFactory(UWeaponProjectileFactory* ProjectileFactory);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;
	UPROPERTY(VisibleAnywhere)
	UWeaponProjectileFactory* AssignedProjectileFactory;
	UPROPERTY(EditAnywhere)
	float DamagePerSecond;
	
	FRotator CurrentRotation;
	FRotator TargetRotation;
	
	UPoseableMeshComponent* TurretMeshComponent;

private:
	void UpdateRotation(float DeltaTime);
	void CreateProjectile(float DeltaTime);
	void InitializeMesh();
	
public:
	virtual void Tick(float DeltaTime) override;
};
