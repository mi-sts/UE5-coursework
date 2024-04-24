
#include "TurretAIController.h"
#include "LestaStart/Core/LestaCharacter.h"

constexpr float SensingInterval = 0.5f;

ATurretAIController::ATurretAIController() : VisibilityDistance(1200.0f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	PawnSensingComponent->SensingInterval = SensingInterval;
	PawnSensingComponent->bOnlySensePlayers = true;
	PawnSensingComponent->SetPeripheralVisionAngle(180.0f);
	PawnSensingComponent->SightRadius = VisibilityDistance;
	PawnSensingComponent->bHearNoises = false;
}

void ATurretAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ATurretAIController::OnSeePawn);
}

void ATurretAIController::BeginPlay()
{
	Super::BeginPlay();
	ControlledTurret = Cast<ATurret>(GetPawn());
	if (!IsValid(ControlledTurret))
	{
		UE_LOG(LogInput, Error, TEXT("The controller turret is not valid!"));
	}
}

void ATurretAIController::Destroyed()
{
	Super::Destroyed();
	PawnSensingComponent->OnSeePawn.RemoveAll(this);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ATurretAIController::OnSeePawn(APawn* OtherPawn)
{
	if (!IsValid(OtherPawn) || !OtherPawn->GetClass()->IsChildOf<ALestaCharacter>() || !IsValid(ControlledTurret))
	{
		IsFollowing = false;
		return;
	}

	IsFollowing = true;
	LastSeenPawn = OtherPawn;
	GetWorldTimerManager().ClearTimer(StopFollowingTimerHandle);
	GetWorldTimerManager().SetTimer(
		StopFollowingTimerHandle,
		this,
		&ATurretAIController::StopFollowing,
		SensingInterval * 3.0f,
		false
	);
	FRotator LookAtPawnRotation = CalculateLookAtRotator(OtherPawn);
	ControlledTurret->RotateTo(LookAtPawnRotation);
}

void ATurretAIController::StopFollowing()
{
	IsFollowing = false;
}

FRotator ATurretAIController::CalculateLookAtRotator(APawn* OtherPawn)
{
	if (!IsValid(OtherPawn))
	{
		UE_LOG(LogInput, Error, TEXT("Cannot calculate look at rotator, the target pawn is not valid!"));
		return FRotator(0.0f);
	}
	
	FVector TurretToPlayerDirection = OtherPawn->GetTargetLocation() - ControlledTurret->GetActorLocation();
	TurretToPlayerDirection.Z = 0.0f;
	return FRotationMatrix::MakeFromX(TurretToPlayerDirection).Rotator();
}

void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsFollowing && IsValid(LastSeenPawn))
	{
		FRotator LookAtPawnRotation = CalculateLookAtRotator(LastSeenPawn);
		ControlledTurret->RotateTo(LookAtPawnRotation);
	}
}
