
#include "Turret.h"

#include "TurretAIController.h"
#include "TurretAnimInstance.h"
#include "LestaStart/Game/Common/HealthComponent.h"
#include "LestaStart/Game/Weapon/LaserWeaponProjectileFactory.h"
#include "LestaStart/UI/HealthbarWidgetComponent.h"
#include "Net/UnrealNetwork.h"

const FName MuzzleSocketName = FName("MuzzleSocket");
const FName TurretRotationBoneName = FName("GunMount");
const FString TurretMeshPath = FString("/Game/Turret/SKM_Turret.SKM_Turret");
const FName TurretAnimInstanceRotationVariableName = FName("TurretRotation");


ATurret::ATurret() : RotationSpeed(10.0f), DamagePerSecond(10.0f), IsShooting(false)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	AActor::SetReplicateMovement(false);
	
	InitializeMesh();

	UWeaponProjectileFactory* InitialProjectileFactory =
		CreateDefaultSubobject<ULaserWeaponProjectileFactory>("ProjectileFactory");
	AssignProjectileFactory(InitialProjectileFactory);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);
	
	AIControllerClass = ATurretAIController::StaticClass();

	TurretMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	TurretMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TurretMeshComponent->SetAnimInstanceClass(UTurretAnimInstance::StaticClass());

	TurretHealthbarWidgetComponent =
		CreateDefaultSubobject<UHealthbarWidgetComponent>("HealthbarWidgetComponent");
	TurretHealthbarWidgetComponent->SetupAttachment(TurretMeshComponent);
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();

	TurretAnimInstance = Cast<UTurretAnimInstance>(TurretMeshComponent->GetAnimInstance());
	if (IsValid(TurretAnimInstance))
	{
		CurrentRotation = TurretAnimInstance->TurretRotation;
		TargetRotation = CurrentRotation;
	}
	
	AddBindings();
}

void ATurret::Destroyed()
{
	Super::Destroyed();
	RemoveBindings();
}

void ATurret::OnDie()
{
	Destroy(true);
}

void ATurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATurret, TargetRotation);
	DOREPLIFETIME(ATurret, IsShooting);
	DOREPLIFETIME(ATurret, RotationSpeed);
	DOREPLIFETIME(ATurret, DamagePerSecond);
	DOREPLIFETIME(ATurret, AssignedProjectileFactory);
}

void ATurret::AddBindings()
{
	HealthComponent->OnHealthChanged().AddUObject(this, &ATurret::OnHealthChanged);
}

void ATurret::RemoveBindings()
{
	HealthComponent->OnHealthChanged().RemoveAll(this);
}

void ATurret::OnHealthChanged(float CurrentHealth)
{
	if (IsValid(TurretHealthbarWidgetComponent) && IsValid(HealthComponent))
		TurretHealthbarWidgetComponent->SetHealthPercent(CurrentHealth / HealthComponent->GetMaxHealth());
	
	if (CurrentHealth <= 0.0f)
	{
		OnDead();
	}
}

void ATurret::OnDead()
{
	Destroy();
}

void ATurret::InitializeMesh()
{
	TurretMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> TurretMeshFinder(TEXT("/Game/Turret/SKM_Turret.SKM_Turret"));
	if (TurretMeshFinder.Succeeded())
	{
		TurretMeshComponent->SetSkinnedAssetAndUpdate(TurretMeshFinder.Object);
		TurretMeshComponent->SetRelativeLocation(FVector(0.0f));
		TurretMeshComponent->SetWorldScale3D(FVector(1.0f));
	}
}

void ATurret::RotateTo(FRotator Rotation)
{
	Rotation.Yaw -= 90.0f;
	TargetRotation = Rotation;
}

void ATurret::AssignProjectileFactory(UWeaponProjectileFactory* ProjectileFactory)
{
	if (!IsValid(ProjectileFactory))
		return;

	if (IsValid(AssignedProjectileFactory))
	{
		AssignedProjectileFactory->Deactivate();
		AssignedProjectileFactory->SetIsReplicated(false);
		AssignedProjectileFactory->DestroyComponent();
	}

	ProjectileFactory->SetIsReplicated(true);
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

void ATurret::StartShooting()
{
	IsShooting = true;
}

void ATurret::StopShooting()
{
	IsShooting = false;
}

void ATurret::UpdateRotation(float DeltaTime)
{
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
	if (IsValid(TurretAnimInstance))
		TurretAnimInstance->TurretRotation = NewRotation;
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
	if (IsShooting)
		CreateProjectile(DeltaTime);
}
