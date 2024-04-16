
#include "WeaponClipSystem.h"

#include "algorithm"


UWeaponClipSystem::UWeaponClipSystem(): ClipCapacity(0), TotalAmmo(0), ClipAmmo(0)
{
	PrimaryComponentTick.bCanEverTick = true;
}

int32 UWeaponClipSystem::GetTotalAmmo()
{
	return TotalAmmo;
}

int32 UWeaponClipSystem::GetClipAmmo()
{
	return ClipAmmo;
}


void UWeaponClipSystem::BeginPlay()
{
	Super::BeginPlay();
}


void UWeaponClipSystem::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWeaponClipSystem::HaveClipAmmo()
{
	return ClipAmmo > 0;
}

void UWeaponClipSystem::Reload()
{
	if (ClipAmmo == ClipCapacity)
	{
		UE_LOG(LogInput, Log, TEXT("The weapon is already reloaded."));
		return;
	}

	int AddedClipAmmo = std::min(ClipCapacity - ClipAmmo, TotalAmmo);
	TotalAmmo -= AddedClipAmmo;

	UE_LOG(LogInput, Warning, TEXT("The weapon has been reloaded."));
}

