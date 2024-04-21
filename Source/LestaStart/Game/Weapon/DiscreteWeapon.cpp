

#include "DiscreteWeapon.h"

ADiscreteWeapon::ADiscreteWeapon(): ShotsDelay(0.125f), ShotDamage(10.0f)
{
	ShotFuncDelegate = FTimerDelegate::CreateUObject(this, &ADiscreteWeapon::TakeShot, ShotDamage);
}

void ADiscreteWeapon::PullTrigger()
{
	Super::PullTrigger();

	GetWorldTimerManager().SetTimer(
		ShotTimerHandle,
		ShotFuncDelegate,
		ShotsDelay,
		true
	);
}

void ADiscreteWeapon::ReleaseTrigger()
{
	Super::ReleaseTrigger();
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ADiscreteWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADiscreteWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

