
#include "WeaponAmmoSystem.h"


UWeaponAmmoSystem::UWeaponAmmoSystem(): ClipCapacity(0), TotalAmmo(0), ClipAmmo(0), HaveInfiniteAmmo(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}

int32 UWeaponAmmoSystem::GetTotalAmmo()
{
	return TotalAmmo;
}

int32 UWeaponAmmoSystem::GetClipAmmo()
{
	return ClipAmmo;
}

void UWeaponAmmoSystem::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponAmmoSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWeaponAmmoSystem::HaveClipAmmo()
{
	return ClipAmmo > 0;
}

void UWeaponAmmoSystem::Reload()
{
	if (ClipAmmo == ClipCapacity)
	{
		UE_LOG(LogInput, Log, TEXT("The weapon is already reloaded."));
		return;
	}

	int AddedClipAmmo = FGenericPlatformMath::Min(ClipCapacity - ClipAmmo, TotalAmmo);
	TotalAmmo -= AddedClipAmmo;

	UE_LOG(LogInput, Warning, TEXT("The weapon has been reloaded."));
}

