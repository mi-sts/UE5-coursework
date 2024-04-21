﻿
#include "WeaponLaserProjectileFactory.h"

#include "Engine/SkeletalMeshSocket.h"


UWeaponLaserProjectileFactory::UWeaponLaserProjectileFactory()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponLaserProjectileFactory::CreateProjectile()
{
}

void UWeaponLaserProjectileFactory::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponLaserProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PlayerCameraTransformGetter == nullptr ||
		WeaponMuzzleTransformGetter == nullptr)
	{
		UE_LOG(LogInput, Error, TEXT("Unable to create a laser projectile!"));
		return;
	}

	FTransform MuzzleTransform = WeaponMuzzleTransformGetter();
	FTransform CameraTransform = PlayerCameraTransformGetter();
	UE_LOG(LogInput, Log, TEXT("%f"), MuzzleTransform.GetLocation().X);
	FVector LaserStartPoint = MuzzleTransform.GetLocation();
	FVector LaserEndPoint = LaserStartPoint + CameraTransform.GetRotation().GetForwardVector() * LaserHitDistance;
	FHitResult LaserHitResult;
	if (GetWorld()->LineTraceSingleByChannel(LaserHitResult, LaserStartPoint, LaserEndPoint, ECC_Visibility))
	{
		LaserEndPoint = LaserHitResult.Location;
	}
	DrawDebugLine(GetWorld(), LaserStartPoint, LaserEndPoint, FColor::Purple);
}

