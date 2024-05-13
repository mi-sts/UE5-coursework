

#include "SphereWeapon.h"

#include "SphereWeaponProjectileFactory.h"

ASphereWeapon::ASphereWeapon():
	MaxChargeValue(50.0f),
	ChargePerSecondIncrease(10.0f),
	DamageRadius(1500.0f),
	CurrentChargeValue(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	USphereWeaponProjectileFactory* SphereProjectileFactory =
		CreateDefaultSubobject<USphereWeaponProjectileFactory>(TEXT("ProjectileFactory"));
	SphereProjectileFactory->Initialize(DamageRadius);
	ProjectileFactory = SphereProjectileFactory;
	ProjectileFactory->SetIsReplicated(true);
	
	InitializeMesh(FString("/Game/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol"));
}

void ASphereWeapon::PullTrigger()
{
	Super::PullTrigger();
	CurrentChargeValue = 0.0f;
}

void ASphereWeapon::ReleaseTrigger()
{
	Super::ReleaseTrigger();
	TakeShot(CurrentChargeValue);
	CurrentChargeValue = 0.0f;
}

void ASphereWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASphereWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsTriggered)
	{
		CurrentChargeValue = FGenericPlatformMath::Min(
			CurrentChargeValue + ChargePerSecondIncrease * DeltaTime,
			MaxChargeValue
		);
	}
}

