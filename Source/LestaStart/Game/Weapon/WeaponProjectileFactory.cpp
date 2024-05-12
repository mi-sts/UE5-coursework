
#include "WeaponProjectileFactory.h"

#include "Net/UnrealNetwork.h"


UWeaponProjectileFactory::UWeaponProjectileFactory(): WeaponMuzzleTransformGetter(nullptr),
                                                      PlayerCameraTransformGetter(nullptr),
                                                      IsCreationEnabled(false)
{
}

void UWeaponProjectileFactory::CreateProjectile(float Damage)
{
	if (!IsCreationEnabled)
	{
		UE_LOG(LogInput, Error, TEXT("Cannot create a projectile, creation is disabled!"));
		return;
	}

	OnProjectileCreation(Damage);
}

void UWeaponProjectileFactory::EnableCreation()
{
	IsCreationEnabled = true;
}

void UWeaponProjectileFactory::DisableCreation()
{
	IsCreationEnabled = false;
}

void UWeaponProjectileFactory::Initialize(
	TFunction<FTransform()> MuzzleTransformGetter,
	TFunction<FTransform()> CameraTransformGetter
)
{
	WeaponMuzzleTransformGetter = MuzzleTransformGetter;
	PlayerCameraTransformGetter = CameraTransformGetter;
}

void UWeaponProjectileFactory::BeginPlay()
{
	Super::BeginPlay();
}

AActor* UWeaponProjectileFactory::GetWeaponOwner()
{
	if (IsValid(GetOwner())) {
		if (IsValid(GetOwner()->GetAttachParentActor()))
			return GetOwner()->GetAttachParentActor();
		
		return GetOwner();
	}
	
	return nullptr;
}

void UWeaponProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

