﻿
#include "LaserWeapon.h"

#include "WeaponLaserProjectileFactory.h"


ALaserWeapon::ALaserWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileFactory = CreateDefaultSubobject<UWeaponLaserProjectileFactory>(TEXT("ProjectileFactory"));
	
	InitializeMesh(FString("/Game/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol"));
}

void ALaserWeapon::Activate(UCameraComponent* CameraComponent)
{
	Super::Activate(CameraComponent);
	ProjectileFactory->Initialize(
		[&]() { return GetMuzzleTransform(); },
		[&]() { return GetCameraTransform(); }
	);
}

void ALaserWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

