// Fill out your copyright notice in the Description page of Project Settings.

#include "LestaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "LestaStart/Game/Weapon/LaserWeapon.h"
#include "LestaStart/Game/Weapon/SphereWeapon.h"

ALestaCharacter::ALestaCharacter()
{
	NetUpdateFrequency = 10.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->bUsePawnControlRotation = true; // Camera rotation is synchronized with Player Controller rotation
	CameraComponent->SetupAttachment(GetMesh());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	WeaponSocketTransform = GetMesh()->GetSocketTransform(WeaponSocketName, RTS_World);
}

void ALestaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EIC) // should validate component because developers can change input component class through Project Settings
	{
		EIC->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnMoveInput);
		EIC->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnLookInput);
		EIC->BindAction(ShootInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnShootInput);
		EIC->BindAction(FirstWeaponInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnFirstWeaponInput);
		EIC->BindAction(SecondWeaponInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnSecondWeaponInput);
	}
	else
	{
		// Print error message into log
		// You can read more here: https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine
		UE_LOG(LogInput, Error, TEXT("Unexpected input component class: %s"), *GetFullNameSafe(PlayerInputComponent))
	}
}

float ALestaCharacter::GetHealth()
{
	if (!IsValid(HealthComponent))
	{
		UE_LOG(LogInput, Error, TEXT("The character health component is not valid!"));
		return 0.0f;
	}

	return HealthComponent->GetHealth();
}

void ALestaCharacter::BeginPlay()
{
	Super::BeginPlay();
	FirstWeapon = SpawnWeapon(ASphereWeapon::StaticClass());
	SecondWeapon = SpawnWeapon(ALaserWeapon::StaticClass());
	AttachWeapon(SecondWeapon);
	
	AddBindings();
}

void ALestaCharacter::AddBindings()
{
	HealthComponent->OnHealthChanged().AddUObject(this, &ALestaCharacter::OnHealthChanged);
}

void ALestaCharacter::RemoveBindings()
{
	HealthComponent->OnHealthChanged().RemoveAll(this);
}

void ALestaCharacter::Destroyed()
{
	RemoveBindings();
}

void ALestaCharacter::OnMoveInput(const FInputActionInstance& InputActionInstance)
{
	// Controller rotation Yaw determines which direction Character is facing
	// so MoveForward = along that direction AND MoveRight = to the right of that direction

	const float YawDegree = GetControlRotation().Yaw; // controller rotation Yaw (in degrees)
	const float YawRadian = FMath::DegreesToRadians(YawDegree); // controller rotation Yaw (in radians)
	const FVector ForwardDirection = FVector(FMath::Cos(YawRadian), FMath::Sin(YawRadian), 0.f);
	const FVector RightDirection = FVector(FMath::Cos(YawRadian + UE_HALF_PI), FMath::Sin(YawRadian + UE_HALF_PI), 0.f);

	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddMovementInput(ForwardDirection * Input2D.X + RightDirection * Input2D.Y);
}

void ALestaCharacter::OnLookInput(const FInputActionInstance& InputActionInstance)
{
	const FVector2D Input2D = InputActionInstance.GetValue().Get<FVector2D>();
	AddControllerYawInput(Input2D.X);
	AddControllerPitchInput(Input2D.Y);
}

void ALestaCharacter::OnShootInput(const FInputActionInstance& InputActionInstance)
{
	if (!IsValid(AttachedWeapon))
		return;
	
	const bool IsPressed = InputActionInstance.GetValue().Get<bool>();
	if (IsPressed)
	{
		AttachedWeapon->PullTrigger();
	}
	else
	{
		AttachedWeapon->ReleaseTrigger();
	}
}

void ALestaCharacter::OnFirstWeaponInput(const FInputActionInstance& InputActionInstance)
{
	const bool IsPressed = InputActionInstance.GetValue().Get<bool>();
	if (!IsPressed)
		return;

	AttachWeapon(FirstWeapon);
}

void ALestaCharacter::OnSecondWeaponInput(const FInputActionInstance& InputActionInstance)
{
	const bool IsPressed = InputActionInstance.GetValue().Get<bool>();
	if (!IsPressed)
		return;

	AttachWeapon(SecondWeapon);
}

void ALestaCharacter::OnHealthChanged(float CurrentHealth)
{
	if (CurrentHealth <= 0.0f)
	{
		OnDead();
	}
}

void ALestaCharacter::OnDead()
{
	GetMesh()->SetSimulatePhysics(true);
	GetController()->GetPawn()->DisableInput(nullptr);
}

AWeapon* ALestaCharacter::SpawnWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	if (IsValid(SpawnedWeapon))
	{
		SpawnedWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			WeaponSocketName
		);
	}

	return SpawnedWeapon;
}

void ALestaCharacter::AttachWeapon(AWeapon* AttachingWeapon)
{
	if (!IsValid(AttachingWeapon))
	{
		UE_LOG(LogInput, Error, TEXT("The attaching weapon is not valid!"));
		return;
	}

	if (IsValid(AttachedWeapon))
		AttachedWeapon->Deactivate();
	
	AttachedWeapon = AttachingWeapon;

	AttachedWeapon->Activate(CameraComponent);
}
