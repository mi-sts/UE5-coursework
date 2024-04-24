
#include "Turret.h"
#include "LestaStart/Game/Weapon/LaserWeaponProjectileFactory.h"


ATurret::ATurret() : RotationSpeed(10.0f), DamagePerSecond(10.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentRotation = GetActorRotation();
	TargetRotation = CurrentRotation;

	UWeaponProjectileFactory* InitialProjectileFactory =
		CreateDefaultSubobject<ULaserWeaponProjectileFactory>("ProjectileFactory");
	AssignProjectileFactory(InitialProjectileFactory);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
}

void ATurret::RotateTo(FRotator Rotation)
{
	TargetRotation = Rotation;
}

void ATurret::AssignProjectileFactory(UWeaponProjectileFactory* ProjectileFactory)
{
	if (!IsValid(ProjectileFactory))
		return;

	if (IsValid(AssignedProjectileFactory))
	{
		AssignedProjectileFactory->Deactivate();
		AssignedProjectileFactory->DestroyComponent();
	}
	
	AssignedProjectileFactory = ProjectileFactory;
	AssignedProjectileFactory->Activate();
}

void ATurret::UpdateRotation(float DeltaTime)
{
	FRotator NewRotation = FMath::RInterpTo( CurrentRotation, TargetRotation, DeltaTime, RotationSpeed );
	SetActorRotation(NewRotation);
	CurrentRotation = NewRotation;
}

void ATurret::CreateProjectile(float DeltaTime)
{
	if (!IsValid(AssignedProjectileFactory))
		return;

	AssignedProjectileFactory->CreateProjectile(DamagePerSecond * DeltaTime);
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateRotation(DeltaTime);
	CreateProjectile(DeltaTime);
}
