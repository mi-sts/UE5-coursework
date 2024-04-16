
#include "WeaponProjectileFactory.h"


UWeaponProjectileFactory::UWeaponProjectileFactory()
{
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

