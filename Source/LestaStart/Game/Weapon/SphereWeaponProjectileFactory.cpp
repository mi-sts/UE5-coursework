// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWeaponProjectileFactory.h"


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
	DrawDebugSphere(
		GetWorld(),
		CameraTransform.GetLocation(),
		DamageRadius, 
		100,
		FColor::Red,
		false,
		3.0f
	);
}

void USphereWeaponProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

