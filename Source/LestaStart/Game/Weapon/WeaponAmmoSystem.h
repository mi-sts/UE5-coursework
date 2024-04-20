// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponAmmoSystem.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class LESTASTART_API UWeaponAmmoSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponAmmoSystem();
	int32 GetTotalAmmo();
	int32 GetClipAmmo();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int ClipCapacity;
	UPROPERTY(EditDefaultsOnly)
	int TotalAmmo;
	UPROPERTY(VisibleAnywhere)
	int ClipAmmo;

	bool HaveInfiniteAmmo;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	bool HaveClipAmmo();
	void Reload();
};
