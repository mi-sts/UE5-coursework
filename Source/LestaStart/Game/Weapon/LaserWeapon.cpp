
#include "LaserWeapon.h"

#include "LaserWeaponProjectileFactory.h"


ALaserWeapon::ALaserWeapon() : DamagePerSecond(10.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileFactory = CreateDefaultSubobject<ULaserWeaponProjectileFactory>(TEXT("ProjectileFactory"));
	InitializeMesh(FString("/Game/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol"));
}

void ALaserWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsTriggered)
		TakeShot(DamagePerSecond * DeltaTime);
}

