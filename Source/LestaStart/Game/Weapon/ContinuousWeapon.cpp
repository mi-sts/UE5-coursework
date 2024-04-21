
#include "ContinuousWeapon.h"


AContinuousWeapon::AContinuousWeapon()
{
}

void AContinuousWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AContinuousWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsTriggered)
		TakeShot(DamagePerSecond * DeltaTime);
}

