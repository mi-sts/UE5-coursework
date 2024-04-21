// Fill out your copyright notice in the Description page of Project Settings.

#include "LestaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "LestaStart/Game/Weapon/LaserWeapon.h"

ALestaCharacter::ALestaCharacter()
{
	NetUpdateFrequency = 10.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->bUsePawnControlRotation = true; // Camera rotation is synchronized with Player Controller rotation
	CameraComponent->SetupAttachment(GetMesh());

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
	}
	else
	{
		// Print error message into log
		// You can read more here: https://dev.epicgames.com/documentation/en-us/unreal-engine/logging-in-unreal-engine
		UE_LOG(LogInput, Error, TEXT("Unexpected input component class: %s"), *GetFullNameSafe(PlayerInputComponent))
	}
}
void ALestaCharacter::BeginPlay()
{
	Super::BeginPlay();
	Weapon = GetWorld()->SpawnActor<ALaserWeapon>();
	AttachWeapon(Weapon);
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
	const bool IsPressed = InputActionInstance.GetValue().Get<bool>();
	if (IsPressed)
	{
		Weapon->PullTrigger();
	}
	else
	{
		Weapon->ReleaseTrigger();
	}
}

void ALestaCharacter::AttachWeapon(AWeapon* AttachingWeapon)
{
	if (AttachingWeapon == nullptr || !IsValid(AttachingWeapon))
	{
		UE_LOG(LogInput, Error, TEXT("The attaching weapon is not valid!"));
		return;
	}

	AttachingWeapon->AttachToComponent(
		GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		WeaponSocketName
	);
	AttachingWeapon->Initialize(CameraComponent);
}
