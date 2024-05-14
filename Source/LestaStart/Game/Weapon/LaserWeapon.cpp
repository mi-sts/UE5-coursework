
#include "LaserWeapon.h"

#include "LaserWeaponProjectileFactory.h"
#include "Net/UnrealNetwork.h"

const FString LaserWeaponMeshPath = FString("/Game/Weapons/Pistol/Mesh/SK_Pistol.SK_Pistol");

ALaserWeapon::ALaserWeapon() : DamagePerSecond(10.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileFactory = CreateDefaultSubobject<ULaserWeaponProjectileFactory>(TEXT("ProjectileFactory"));
	ProjectileFactory->SetIsReplicated(true);
	InitializeMesh(LaserWeaponMeshPath);
}

void ALaserWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALaserWeapon, DamagePerSecond);
}

void ALaserWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ALaserWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsTriggered)
	{
		float Damage = DamagePerSecond * DeltaTime;
		if (HasAuthority())
		{
			TakeShot(Damage);
		}
		else
		{
			APawn* WeaponOwner = GetWeaponOwner();
			if (IsValid(WeaponOwner) && WeaponOwner->IsLocallyControlled())
				ProjectileFactory->CreateProjectileView(Damage);
		}
	}
}

