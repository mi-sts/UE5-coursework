
#include "LaserWeaponProjectileFactory.h"


ULaserWeaponProjectileFactory::ULaserWeaponProjectileFactory()
{
}

void ULaserWeaponProjectileFactory::OnProjectileCreation(float Damage)
{
	if (PlayerCameraTransformGetter == nullptr ||
		WeaponMuzzleTransformGetter == nullptr)
	{
		UE_LOG(LogInput, Error, TEXT("Unable to create a laser projectile!"));
		return;
	}
	
	FTransform MuzzleTransform = WeaponMuzzleTransformGetter();
	FTransform CameraTransform = PlayerCameraTransformGetter();
	FVector LaserStartPoint = MuzzleTransform.GetLocation();
	FVector LaserEndPoint = LaserStartPoint + CameraTransform.GetRotation().GetForwardVector() * LaserHitDistance;
	FHitResult LaserHitResult;
	if (GetWorld()->LineTraceSingleByChannel(LaserHitResult, LaserStartPoint, LaserEndPoint, ECC_Visibility))
	{
		LaserEndPoint = LaserHitResult.Location;
	}
	DrawDebugLine(GetWorld(), LaserStartPoint, LaserEndPoint, FColor::Purple);
}
