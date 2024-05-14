
#include "WeaponProjectileFactory.h"

#include "Net/UnrealNetwork.h"

UWeaponProjectileFactory::UWeaponProjectileFactory(): WeaponMuzzleTransformGetter(nullptr),
                                                      PlayerCameraTransformGetter(nullptr),
                                                      IsCreationEnabled(false)
{
}

void UWeaponProjectileFactory::CreateProjectile(float Damage)
{
	CreateProjectileView(Damage);
	ServerCreateProjectile(Damage);
}

void UWeaponProjectileFactory::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponProjectileFactory, IsCreationEnabled);
}


void UWeaponProjectileFactory::ServerCreateProjectile_Implementation(float Damage)
{
	if (!IsCreationEnabled)
	{
		UE_LOG(LogInput, Error, TEXT("Cannot create a projectile, creation is disabled!"));
		return;
	}

	OnServerProjectileCreation(Damage);
	MulticastCreateProjectileView(Damage);
}

void UWeaponProjectileFactory::EnableCreation()
{
	IsCreationEnabled = true;
}

void UWeaponProjectileFactory::DisableCreation()
{
	IsCreationEnabled = false;
}

void UWeaponProjectileFactory::Initialize(
	TFunction<FTransform()> MuzzleTransformGetter,
	TFunction<FTransform()> CameraTransformGetter
)
{
	WeaponMuzzleTransformGetter = MuzzleTransformGetter;
	PlayerCameraTransformGetter = CameraTransformGetter;
}

void UWeaponProjectileFactory::BeginPlay()
{
	Super::BeginPlay();
}

void UWeaponProjectileFactory::OnServerProjectileCreation(float Damage)
{
}

void UWeaponProjectileFactory::CreateProjectileView(float Damage)
{
}


void UWeaponProjectileFactory::MulticastCreateProjectileView_Implementation(float Damage)
{
	if (!IsValid(GetWeaponOwner()))
		return;

	if (GetWeaponOwner()->HasAuthority())
		return;

	CreateProjectileView(Damage);
}

AActor* UWeaponProjectileFactory::GetWeaponOwner()
{
	if (IsValid(GetOwner())) {
		if (IsValid(GetOwner()->GetAttachParentActor()))
			return GetOwner()->GetAttachParentActor();
		
		return GetOwner();
	}
	
	return nullptr;
}

void UWeaponProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

