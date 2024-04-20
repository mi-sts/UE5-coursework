
#include "Weapon.h"


AWeapon::AWeapon(): IsTriggered(false)
{
	WeaponAmmoSystem = CreateDefaultSubobject<UWeaponAmmoSystem>(TEXT("AmmoSystem"));
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMeshComponent->SetupAttachment(RootComponent);
}

void AWeapon::PullTrigger()
{
	IsTriggered = true;
	UE_LOG(LogInput, Warning, TEXT("Trigger pulled"));
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AWeapon::TakeShot, 1.0f, true);
}

void AWeapon::ReleaseTrigger()
{
	IsTriggered = false;
	UE_LOG(LogInput, Warning, TEXT("Trigger released"));
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::InitializeMesh(const FString& MeshReferenceName)
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMesh(*MeshReferenceName);
	if (WeaponMesh.Succeeded())
	{
		WeaponMeshComponent->SetSkeletalMesh(WeaponMesh.Object);
		WeaponMeshComponent->SetRelativeLocation(FVector(0.0f));
		WeaponMeshComponent->SetWorldScale3D(FVector(1.0f));
	}
}

void AWeapon::TakeShot()
{
	UE_LOG(LogInput, Warning, TEXT("Shot!"));
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

