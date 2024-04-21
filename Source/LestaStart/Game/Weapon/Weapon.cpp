
#include "Weapon.h"

#include "Engine/SkeletalMeshSocket.h"


AWeapon::AWeapon(): IsTriggered(false)
{
	WeaponAmmoSystem = CreateDefaultSubobject<UWeaponAmmoSystem>(TEXT("AmmoSystem"));
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMeshComponent->SetupAttachment(RootComponent);
}

void AWeapon::PullTrigger()
{
	IsTriggered = true;
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AWeapon::TakeShot, 1.0f, true);
}

void AWeapon::ReleaseTrigger()
{
	IsTriggered = false;
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void AWeapon::Initialize(UCameraComponent* CameraComponent)
{
	PlayerCameraComponent = CameraComponent;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::InitializeMesh(const FString& MeshReferenceName)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshFinder(*MeshReferenceName);
	if (WeaponMeshFinder.Succeeded())
	{
		WeaponMeshComponent->SetSkeletalMesh(WeaponMeshFinder.Object);
		WeaponMeshComponent->SetRelativeLocation(FVector(0.0f));
		WeaponMeshComponent->SetWorldScale3D(FVector(1.0f));
		
		MuzzleSocket = WeaponMeshFinder.Object->FindSocket(TEXT("MuzzleSocket"));
	}
}

FTransform AWeapon::GetMuzzleTransform()
{
	if (!IsValid(MuzzleSocket) || !IsValid((WeaponMeshComponent)))
	{
		UE_LOG(LogInput, Error, TEXT("Cannot get the weapon muzzle transform!"));
		return FTransform();
	}
	
	return MuzzleSocket->GetSocketTransform(WeaponMeshComponent);
}

FTransform AWeapon::GetCameraTransform()
{
	if (!IsValid(PlayerCameraComponent))
	{
		UE_LOG(LogInput, Error, TEXT("Cannot get the player camera transform!"));
		return FTransform();
	}
	
	return PlayerCameraComponent->GetComponentTransform();
}

void AWeapon::TakeShot()
{
	ProjectileFactory->CreateProjectile();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

