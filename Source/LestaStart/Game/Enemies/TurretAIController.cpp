
#include "TurretAIController.h"
#include "LestaStart/Core/LestaCharacter.h"

ATurretAIController::ATurretAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
	PawnSensingComponent->SensingInterval = 0.05f;
	PawnSensingComponent->bOnlySensePlayers = true;
	PawnSensingComponent->SetPeripheralVisionAngle(90.0f);
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
}

void ATurretAIController::OnSeePawn(APawn* OtherPawn)
{
	if (!IsValid(OtherPawn) || !OtherPawn->GetClass()->IsChildOf<ALestaCharacter>())
		return;

	if (!IsValid(ControlledTurret))
		return;

	FVector TurretToPlayerDirection = OtherPawn->GetTargetLocation() - ControlledTurret->GetActorLocation();
	FRotator LookAtPlayerRotation = FRotationMatrix::MakeFromZ(TurretToPlayerDirection).Rotator();
	ControlledTurret->RotateTo(LookAtPlayerRotation);
}
