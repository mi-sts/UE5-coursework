﻿
#include "Weapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "LestaStart/Core/LestaCharacter/LestaCharacter.h"
#include "Net/UnrealNetwork.h"

const FName WeaponMuzzleSocketName = FName("MuzzleSocket");

AWeapon::AWeapon(): IsTriggered(false)
{
	bReplicates = true;
	
	WeaponAmmoSystem = CreateDefaultSubobject<UWeaponAmmoSystem>(TEXT("AmmoSystem"));
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMeshComponent->SetupAttachment(RootComponent);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, IsTriggered);
	DOREPLIFETIME(AWeapon, IsVisible);
	DOREPLIFETIME(AWeapon, ProjectileFactory);
	DOREPLIFETIME(AWeapon, PlayerCameraComponent);
	DOREPLIFETIME(AWeapon, MuzzleSocket);
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
	if (IsValid(WeaponMeshComponent) && IsValid(WeaponMeshComponent->GetSkeletalMeshAsset()))
	{
		MuzzleSocket = WeaponMeshComponent->GetSkeletalMeshAsset()->FindSocket(WeaponMuzzleSocketName);
	}
	
	ProjectileFactory->Initialize(
	[&]() { return GetMuzzleTransform(); },
	[&]() { return GetCameraTransform(); }
	);
	SetVisible(true);
	ProjectileFactory->EnableCreation();
}

void AWeapon::Deactivate()
{
	ProjectileFactory->DisableCreation();
	SetVisible(false);
}

void AWeapon::SetVisible(bool Visible)
{
	if (Visible == IsVisible)
		return;

	WeaponMeshComponent->SetVisibility(Visible);
	IsVisible = Visible;
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
		
		MuzzleSocket = WeaponMeshFinder.Object->FindSocket(WeaponMuzzleSocketName);
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

APawn* AWeapon::GetWeaponOwner()
{
	return Cast<APawn>(GetAttachParentActor());
}

void AWeapon::TakeShot(float Damage)
{
	if (!IsValid(GetAttachParentActor()) || !IsValid(ProjectileFactory))
		return;

	ALestaCharacter* PlayerCharacter = Cast<ALestaCharacter>(GetAttachParentActor());
	if (!IsValid(PlayerCharacter) || !PlayerCharacter->HasAuthority())
		return;
	
	ProjectileFactory->CreateProjectile(Damage);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

