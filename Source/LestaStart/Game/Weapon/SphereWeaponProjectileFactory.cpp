// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereWeaponProjectileFactory.h"

#include "Weapon.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "LestaStart/Utils/DamageSystemUtils.h"


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

void USphereWeaponProjectileFactory::CreateProjectileView(float Damage)
{
	FVector CameraLocation = PlayerCameraTransformGetter().GetLocation();
	DrawDebugSphere(
		GetWorld(),
		CameraLocation,
		DamageRadius, 
		30,
		FColor::Red,
		false,
		3.0f
	);
}

void USphereWeaponProjectileFactory::OnServerProjectileCreation(float Damage)
{
	ServerApplyRadialDamage(Damage);
}

void USphereWeaponProjectileFactory::ServerApplyRadialDamage_Implementation(float Damage)
{
	FVector CameraLocation = PlayerCameraTransformGetter().GetLocation();
	AActor* WeaponOwner = GetWeaponOwner();
	
	TArray<AActor*> OverlappedActors;
	if (UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		CameraLocation,
		DamageRadius,
		{},
		AActor::StaticClass(),
		{ WeaponOwner },
		OverlappedActors
	))
	{
		for (AActor* OverlappedActor : OverlappedActors)
		{
			if (!IsValid(OverlappedActor))
				continue;
			
			if (OverlappedActor->GetClass() == WeaponOwner->GetClass())
				continue;

			FDamageEvent DamageEvent;
			OverlappedActor->TakeDamage(Damage, DamageEvent, nullptr, WeaponOwner);
			DamageSystemUtils::CheckActorDeath(OverlappedActor);
		}
	}
}

void USphereWeaponProjectileFactory::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

