
#include "TurretAIController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "LestaStart/Core/LestaCharacter.h"
#include "LestaStart/Utils/TransformUtils.h"

ATurretAIController::ATurretAIController() : DetectingInterval(0.5f), VisibilityDistance(1200.0f), IsFollowing(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATurretAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetWorldTimerManager().SetTimer(
		DetectionTimerHandle,
		this,
		&ATurretAIController::UpdateVisibleActors,
		DetectingInterval,
		true
	);
	UpdateVisibleActors();
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
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void ATurretAIController::OnDetectActor(AActor* OtherActor)
{
	if (!IsValid(OtherActor) || !IsValid(ControlledTurret))
	{
		IsFollowing = false;
		return;
	}

	IsFollowing = true;
	LastSeenActor = OtherActor;
	ControlledTurret->StartShooting();
	GetWorldTimerManager().ClearTimer(StopFollowingTimerHandle);
	GetWorldTimerManager().SetTimer(
		StopFollowingTimerHandle,
		this,
		&ATurretAIController::StopFollowing,
		DetectingInterval * 10.0f,
		false
	);
	FRotator LookAtPawnRotation = TransformUtils::CalculateLookAtRotator(
		ControlledTurret->GetActorLocation(),
		OtherActor->GetActorLocation()
	);
	ControlledTurret->RotateTo(LookAtPawnRotation);
}

void ATurretAIController::StopFollowing()
{
	IsFollowing = false;
	ControlledTurret->StopShooting();
}

void ATurretAIController::UpdateVisibleActors()
{
	if (!IsValid(ControlledTurret))
		return;

	VisibleActors.Empty();
	TArray<AActor*> OverlappedActors;
	if (UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		ControlledTurret->GetActorLocation(),
		VisibilityDistance,
		{},
		ALestaCharacter::StaticClass(),
		{},
		OverlappedActors
	))
	{
		for (AActor* OverlappedActor : OverlappedActors)
		{
			if (!IsValid(OverlappedActor))
				return;
			
			VisibleActors.Add(OverlappedActor);
		}
	}

	if (!VisibleActors.IsEmpty())
	{
		if (VisibleActors.Contains(LastSeenActor))
		{
			OnDetectActor(LastSeenActor);
		}
		else
		{
			OnDetectActor(VisibleActors[0]);	
		}
	}
}

void ATurretAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(ControlledTurret) && IsFollowing && IsValid(LastSeenActor))
	{
		FRotator LookAtPawnRotation = TransformUtils::CalculateLookAtRotator(
			ControlledTurret->GetActorLocation(),
			LastSeenActor->GetActorLocation()
		);
		ControlledTurret->RotateTo(LookAtPawnRotation);
	}
}
