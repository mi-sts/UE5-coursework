﻿
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
}

void AWeapon::ReleaseTrigger()
{
	IsTriggered = false;
}

void AWeapon::Activate(UCameraComponent* CameraComponent)
{
	PlayerCameraComponent = CameraComponent;
	ProjectileFactory->EnableCreation();
}

void AWeapon::Deactivate()
{
	ProjectileFactory->DisableCreation();
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

void AWeapon::TakeShot(float Damage)
{
	ProjectileFactory->CreateProjectile(Damage);
	UE_LOG(LogInput, Warning, TEXT("Shot"));
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

