// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWeaponProjectileFactory.h"

#include "Weapon.h"
#include "Kismet/GameplayStatics.h"


USphereWeaponProjectileFactory::USphereWeaponProjectileFactory()
{
	
	PrimaryComponentTick.bCanEverTick = true;
}

void USphereWeaponProjectileFactory::Initialize(float SphereDamageRadius)
{
	DamageRadius = SphereDamageRadius;
}

void USphereWeaponProjectileFactory::BeginPlay()
{
	Super::BeginPlay();
}

void USphereWeaponProjectileFactory::OnProjectileCreation(float Damage)
{
	FTransform CameraTransform = PlayerCameraTransformGetter();
	FVector CameraLocation = CameraTransform.GetLocation() + FVector(100.0, 0.0, 0.0);
	DrawDebugSphere(
		GetWorld(),
		CameraLocation,
		DamageRadius, 
		100,
		FColor::Red,
		false,
		3.0f
	);
	UE_LOG(LogInput, Log, TEXT("QWE"));
	AActor* WeaponOwner = GetWeaponOwner();

	bool Qw = UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		Damage,
		CameraLocation,
		DamageRadius,
		nullptr,
		{ WeaponOwner },
		WeaponOwner,
		nullptr,
		true,
		ECC_Visibility
	);
	UE_LOG(LogInput, Log, TEXT("Damaged %d"), Qw);
}

void USphereWeaponProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

