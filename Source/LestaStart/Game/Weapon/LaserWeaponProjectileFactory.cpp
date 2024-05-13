
#include "LaserWeaponProjectileFactory.h"
#include "Engine/DamageEvents.h"
#include "LestaStart/Utils/DamageSystemUtils.h"


ULaserWeaponProjectileFactory::ULaserWeaponProjectileFactory()
{
}

void ULaserWeaponProjectileFactory::OnServerProjectileCreation(float Damage)
{
	ServerCreateDamageTrace(Damage);
}

void ULaserWeaponProjectileFactory::MulticastCreateProjectileView_Implementation(float Damage)
{
	FHitResult LaserHitResult;

	FVector LaserStartLocation = GetLaserTraceStartLocation();
	FVector LaserEndLocation = LaserStartLocation + GetLaserTraceStartToEndVector();
	
	if (GetLaserTraceHitResult(LaserHitResult))
		LaserEndLocation = LaserHitResult.Location;

	DrawDebugLine(GetWorld(), LaserStartLocation, LaserEndLocation, FColor::Purple);
}

bool ULaserWeaponProjectileFactory::GetLaserTraceHitResult(FHitResult& HitResult)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetWeaponOwner());

	FVector LaserStartLocation = GetLaserTraceStartLocation();
	FVector LaserEndLocation = LaserStartLocation + GetLaserTraceStartToEndVector();
	
	return GetWorld()->LineTraceSingleByChannel(
		HitResult,
		LaserStartLocation,
		LaserEndLocation,
		ECC_Visibility,
		CollisionParams
	);
}

void ULaserWeaponProjectileFactory::ServerCreateDamageTrace_Implementation(float Damage)
{
	FHitResult DamageTraceHitResult;
	if (GetLaserTraceHitResult(DamageTraceHitResult))
	{
		AActor* HitActor = DamageTraceHitResult.GetActor();
		if (IsValid(HitActor))
		{
			FDamageEvent DamageEvent;
			ServerApplyTraceDamage(HitActor, Damage, DamageEvent, GetWeaponOwner());
		}
	}
}

void ULaserWeaponProjectileFactory::ServerApplyTraceDamage_Implementation(
	AActor* ToActor, 
	float Damage,
	FDamageEvent DamageEvent,
	AActor* DamageCauser
)
{
	if (IsValid(ToActor))
		ToActor->TakeDamage(Damage, DamageEvent, nullptr, DamageCauser);
	DamageSystemUtils::CheckActorDeath(ToActor);
}

FVector ULaserWeaponProjectileFactory::GetLaserTraceStartLocation()
{
	if (WeaponMuzzleTransformGetter == nullptr)
	{
		UE_LOG(LogInput, Error, TEXT("The weapon muzzle getter is null!"));
		return FVector(0.0f);
	}
	
	FTransform MuzzleTransform = WeaponMuzzleTransformGetter();
	return MuzzleTransform.GetLocation();
}

FVector ULaserWeaponProjectileFactory::GetLaserTraceStartToEndVector()
{
	if (WeaponMuzzleTransformGetter == nullptr)
	{
		UE_LOG(LogInput, Error, TEXT("The camera transform getter is null!"));
		return FVector(0.0f);
	}
	
	FTransform CameraTransform = PlayerCameraTransformGetter();
	return CameraTransform.GetRotation().GetForwardVector() * LaserHitDistance;
}
