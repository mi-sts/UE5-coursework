// Fill out your copyright notice in the Description page of Project Settings.

#include "LestaCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "LestaStart/Core/LestaGameMode.h"
#include "LestaStart/Game/Weapon/LaserWeapon.h"
#include "LestaStart/Game/Weapon/SphereWeapon.h"
#include "LestaStart/UI/HealthbarWidgetComponent.h"
#include "Net/UnrealNetwork.h"

ALestaCharacter::ALestaCharacter()
{
	bReplicates = true;
	ACharacter::SetReplicateMovement(true);
	
	NetUpdateFrequency = 10.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	CameraComponent->bUsePawnControlRotation = true; // Camera rotation is synchronized with Player Controller rotation
	CameraComponent->SetupAttachment(GetMesh());
	CameraComponent->SetIsReplicated(true);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetIsReplicated(true);

	WeaponSocketTransform = GetMesh()->GetSocketTransform(WeaponSocketName, RTS_World);

	SimulatedPlayerHealthbarWidgetComponent =
		CreateDefaultSubobject<UHealthbarWidgetComponent>("HealthbarWidgetComponent");
	SimulatedPlayerHealthbarWidgetComponent->SetupAttachment(GetMesh());
}

void ALestaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALestaCharacter, AttachedWeapon);
	DOREPLIFETIME(ALestaCharacter, FirstWeapon);
	DOREPLIFETIME(ALestaCharacter, SecondWeapon);
	DOREPLIFETIME(ALestaCharacter, CameraComponent);
	DOREPLIFETIME(ALestaCharacter, HealthComponent);
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
		EIC->BindAction(NextWeaponInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnNextWeaponInput);
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

float ALestaCharacter::GetMaxHealth()
{
	if (!IsValid(HealthComponent))
	{
		UE_LOG(LogInput, Error, TEXT("The character health component is not valid!"));
		return 100.0f;
	}

	return HealthComponent->GetMaxHealth();
}

void ALestaCharacter::BeginPlay()
{
	Super::BeginPlay();
	AddBindings();
	AnimInstance = Cast<ULestaCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (HasAuthority())
		SpawnInitialWeapons();
	ServerRegisterPlayer();
}

void ALestaCharacter::SpawnInitialWeapons()
{
	FirstWeapon = SpawnWeapon(ASphereWeapon::StaticClass());
	SecondWeapon = SpawnWeapon(ALaserWeapon::StaticClass());
	ServerAttachWeapon(SecondWeapon);
}

void ALestaCharacter::AddBindings()
{
	HealthComponent->OnHealthChanged().AddUObject(this, &ALestaCharacter::OnHealthChanged);
}

void ALestaCharacter::RemoveBindings()
{
	HealthComponent->OnHealthChanged().RemoveAll(this);
}

void ALestaCharacter::OnRep_AttachedWeapon()
{
	if (IsValid(AttachedWeapon))
	{
		AttachedWeapon->Activate(CameraComponent);
	}
}

void ALestaCharacter::ServerRegisterPlayer_Implementation()
{
	ALestaGameMode* GameMode = Cast<ALestaGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode))
		return;

	GameMode->ServerRegisterPlayer(this);
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
	float ControlPitch = FMath::ClampAngle(GetControlRotation().Pitch, -90.0f, 90.0f);
	float CameraPitch = CameraComponent->GetComponentRotation().Pitch;
	ServerUpdateCharacterPitch(ControlPitch, CameraPitch);
}

void ALestaCharacter::OnShootInput(const FInputActionInstance& InputActionInstance)
{
	if (!IsValid(AttachedWeapon))
		return;
	
	const bool IsPressed = InputActionInstance.GetValue().Get<bool>();
	if (IsPressed)
	{
		ServerPullTrigger();
	}
	else
	{
		ServerReleaseTrigger();
	}
}

void ALestaCharacter::OnNextWeaponInput(const FInputActionInstance& InputActionInstance)
{
	const float Input = InputActionInstance.GetValue().Get<float>();
	UE_LOG(LogInput, Warning, TEXT("%f"), Input);
}

void ALestaCharacter::MulticastUpdateCharacterPitch_Implementation(float ControlPitch, float CameraPitch)
{
	if (!IsLocallyControlled() && IsValid(AnimInstance))
	{
		AnimInstance->SetPitch(ControlPitch);
		if (IsValid(CameraComponent))
		{
			FRotator CameraRotation = CameraComponent->GetComponentRotation();
			CameraRotation.Pitch = CameraPitch;
			CameraComponent->SetWorldRotation(CameraRotation);
		}
	}
}

void ALestaCharacter::ServerUpdateCharacterPitch_Implementation(float ControlPitch, float CameraPitch)
{
	MulticastUpdateCharacterPitch(ControlPitch, CameraPitch);
}

void ALestaCharacter::ServerPullTrigger_Implementation()
{
	if (!IsValid(AttachedWeapon))
		return;
	
	AttachedWeapon->PullTrigger();
}

void ALestaCharacter::ServerReleaseTrigger_Implementation()
{
	if (!IsValid(AttachedWeapon))
		return;
	
	AttachedWeapon->ReleaseTrigger();
}

void ALestaCharacter::OnFirstWeaponInput(const FInputActionInstance& InputActionInstance)
{
	const bool IsPressed = InputActionInstance.GetValue().Get<bool>();
	if (!IsPressed)
		return;

	ServerAttachWeapon(FirstWeapon);
}

void ALestaCharacter::OnSecondWeaponInput(const FInputActionInstance& InputActionInstance)
{
	const bool IsPressed = InputActionInstance.GetValue().Get<bool>();
	if (!IsPressed)
		return;

	ServerAttachWeapon(SecondWeapon);
}

void ALestaCharacter::OnHealthChanged(float CurrentHealth)
{
	if (IsValid(SimulatedPlayerHealthbarWidgetComponent))
		SimulatedPlayerHealthbarWidgetComponent->SetHealthPercent(GetHealth() / GetMaxHealth());
	
	if (CurrentHealth <= 0.0f)
		Die();
}

void ALestaCharacter::OnDie()
{
	ServerHandleDeath();
	ClientDead();
}

void ALestaCharacter::ServerHandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	ALestaGameMode* GameMode = Cast<ALestaGameMode>(GetWorld()->GetAuthGameMode());
	if (!IsValid(GameMode))
		return;

	GameMode->ServerHandlePlayerDeath(this);
}

void ALestaCharacter::ClientDead_Implementation()
{
	if (IsDead())
		return;
	
	Dead = true;
	GetMesh()->SetSimulatePhysics(true);
	if (IsValid(GetController()) && IsValid(GetController()->GetPawn()))
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

void ALestaCharacter::ServerAttachWeapon_Implementation(AWeapon* AttachingWeapon)
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
