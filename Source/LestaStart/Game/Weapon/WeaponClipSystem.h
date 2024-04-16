// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponClipSystem.generated.h"


UCLASS(meta=(BlueprintSpawnableComponent))
class LESTASTART_API UWeaponClipSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponClipSystem();
	int GetTotalAmmo();
	int GetClipAmmo();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	int ClipCapacity;
	int TotalAmmo;
	int ClipAmmo;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	bool HaveClipAmmo();
	void Reload();
};
