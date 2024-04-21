
#include "WeaponProjectileFactory.h"

#include "Camera/CameraComponent.h"


UWeaponProjectileFactory::UWeaponProjectileFactory(): WeaponMuzzleTransformGetter(nullptr),
                                                      PlayerCameraTransformGetter(nullptr)
{
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

void UWeaponProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

