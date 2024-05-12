// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthbarWidgetComponent.h"

#include "LestaStart/Core/LestaCharacter.h"
#include "LestaStart/Utils/TransformUtils.h"


UHealthbarWidgetComponent::UHealthbarWidgetComponent() : VisibleToPlayer(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetWidgetSpace(EWidgetSpace::World);
}

void UHealthbarWidgetComponent::SetHealthPercent(float Percent)
{
	if (IsValid(HealthbarWidget))
		HealthbarWidget->SetHealthbarPercent(Percent);
}

void UHealthbarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupVisibility();
	if (VisibleToPlayer)
		InitializeWidget();
}

void UHealthbarWidgetComponent::SetupVisibility()
{
	if (IsValid(GetOwner()))
	{
		ALestaCharacter* Character = Cast<ALestaCharacter>(GetOwner());
		if (!IsValid(Character))
		{
			VisibleToPlayer = true;
		}
		else
		{
			VisibleToPlayer = !Character->IsLocallyControlled();	
		}
	}
	
	SetVisibility(VisibleToPlayer);
}

void UHealthbarWidgetComponent::InitializeWidget()
{
	if (!IsValid(HealthbarWidgetClass))
		return;
	
	HealthbarWidget = CreateWidget<UHealthbarWidget>(
		GetWorld(),
		HealthbarWidgetClass
	);
	if (!IsValid(HealthbarWidget))
		return;

	SetWidget(HealthbarWidget);
}

void UHealthbarWidgetComponent::UpdateLookAtPlayerRotation()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!IsValid(PlayerController) || !IsValid(GetOwner()))
		return;
	
	FRotator LookAtPlayerRotation = TransformUtils::CalculateLookAtRotator(
		GetOwner()->GetActorLocation(),
		PlayerController->GetFocalLocation()
	);
	SetWorldRotation(LookAtPlayerRotation);
}


void UHealthbarWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateLookAtPlayerRotation();
}

