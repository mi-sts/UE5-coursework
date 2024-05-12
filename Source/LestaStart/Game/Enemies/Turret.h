// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretAnimInstance.h"
#include "GameFramework/Actor.h"
#include "LestaStart/Game/Common/HealthComponent.h"
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
	UFUNCTION(BlueprintCallable)
	void StartShooting();
	UFUNCTION(BlueprintCallable)
	void StopShooting();
	
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void DOPERLIFETIME() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;
	UPROPERTY(EditAnywhere, Replicated)
	float RotationSpeed;
	UPROPERTY(VisibleAnywhere, Replicated)
	UWeaponProjectileFactory* AssignedProjectileFactory;
	UPROPERTY(EditAnywhere, Replicated)
	float DamagePerSecond;
	
	void AddBindings();
	void RemoveBindings();
	void OnHealthChanged(float CurrentHealth);
	void OnDead();

	UPROPERTY(Replicated)
	bool IsShooting;

	UPROPERTY()
	FRotator CurrentRotation;
	UPROPERTY(Replicated)
	FRotator TargetRotation;
	
	UPROPERTY()
	UTurretAnimInstance* TurretAnimInstance;

private:
	void UpdateRotation(float DeltaTime);
	void CreateProjectile(float DeltaTime);
	void InitializeMesh();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* TurretMeshComponent;
	
	virtual void Tick(float DeltaTime) override;
};
