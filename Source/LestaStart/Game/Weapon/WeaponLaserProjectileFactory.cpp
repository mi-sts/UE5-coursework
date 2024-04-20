
#include "WeaponLaserProjectileFactory.h"


UWeaponLaserProjectileFactory::UWeaponLaserProjectileFactory()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponLaserProjectileFactory::CreateProjectile()
{
	UE_LOG(LogInput, Warning, TEXT("Laser created"));
}

void UWeaponLaserProjectileFactory::BeginPlay()
{
	Super::BeginPlay();
}


void UWeaponLaserProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                                  FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

