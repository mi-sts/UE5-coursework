
#include "Weapon.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::PullTrigger()
{
	UE_LOG(LogInput, Warning, TEXT("Trigger pulled"));
}

void AWeapon::ReleaseTrigger()
{
	UE_LOG(LogInput, Warning, TEXT("Trigger released"));
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

