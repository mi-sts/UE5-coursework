
#include "LaserWeaponProjectileFactory.h"
#include "Engine/DamageEvents.h"


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
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetWeaponOwner());
	
	if (GetWorld()->LineTraceSingleByChannel(
		LaserHitResult,
		LaserStartPoint,
		LaserEndPoint,
		ECC_Visibility,
		CollisionParams
	))
	{
		LaserEndPoint = LaserHitResult.Location;
		AActor* HitActor = LaserHitResult.GetActor();
		if (IsValid(HitActor))
		{
			UE_LOG(LogInput, Error, TEXT("Hit"));
			FDamageEvent DamageEvent;
			HitActor->TakeDamage(Damage, DamageEvent, nullptr, GetWeaponOwner());
		}
	}
	DrawDebugLine(GetWorld(), LaserStartPoint, LaserEndPoint, FColor::Purple);
}
