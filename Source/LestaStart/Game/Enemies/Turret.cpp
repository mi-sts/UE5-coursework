
#include "Turret.h"

#include "Components/PoseableMeshComponent.h"
#include "LestaStart/Game/Weapon/LaserWeaponProjectileFactory.h"

const FName MuzzleSocketName = FName("MuzzleSocket");
const FName TurretRotationBoneName = FName("GunMount");
const FString TurretMeshPath = FString("/Script/Engine.SkeletalMesh'/Game/Turret/Turret.Turret'");


ATurret::ATurret() : RotationSpeed(10.0f), DamagePerSecond(10.0f)
{
	PrimaryActorTick.bCanEverTick = true;

	InitializeMesh();

	CurrentRotation = TurretMeshComponent->GetBoneRotationByName(TurretRotationBoneName, EBoneSpaces::WorldSpace);
	TargetRotation = CurrentRotation;

	UWeaponProjectileFactory* InitialProjectileFactory =
	CreateDefaultSubobject<ULaserWeaponProjectileFactory>("ProjectileFactory");
	AssignProjectileFactory(InitialProjectileFactory);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
}

void ATurret::InitializeMesh()
{
	TurretMeshComponent = CreateDefaultSubobject<UPoseableMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TurretMeshFinder(TEXT("/Game/Turret/Turret.Turret"));
	if (TurretMeshFinder.Succeeded())
	{
		TurretMeshComponent->SetSkinnedAssetAndUpdate(TurretMeshFinder.Object);
		TurretMeshComponent->SetRelativeLocation(FVector(0.0f));
		TurretMeshComponent->SetWorldScale3D(FVector(1.0f));
	}
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
	auto GetMuzzleTransform = [&](){ 
		FTransform MuzzleTransform = TurretMeshComponent->GetSocketTransform(MuzzleSocketName);
		FRotator CorrectionRotator = FRotator(0.0f, 90.0f, 0.0f);
		MuzzleTransform.SetRotation((MuzzleTransform.Rotator() + CorrectionRotator).Quaternion());

		return MuzzleTransform;
	};
	AssignedProjectileFactory->Initialize(
		GetMuzzleTransform,
		GetMuzzleTransform
	); 
	AssignedProjectileFactory->EnableCreation();
}

void ATurret::UpdateRotation(float DeltaTime)
{
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
	FRotator BoneRotation = FRotator(NewRotation);
	BoneRotation.Roll = 90.0f;
	BoneRotation.Yaw -= 90.0f;	
	
	TurretMeshComponent->SetBoneRotationByName(TurretRotationBoneName, BoneRotation, EBoneSpaces::WorldSpace);
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
