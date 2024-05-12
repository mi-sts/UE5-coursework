
#include "LestaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "LestaStart/UI/PlayerUIWidget.h"

void ALestaPlayerController::InitializeLocalPlayerUI()
{
	if (PlayerUIWidgetClass)
	{
		PlayerUIWidget = CreateWidget<UPlayerUIWidget>(
			Cast<APlayerController>(this),
			PlayerUIWidgetClass
		);
		if (IsValid(PlayerUIWidget))
			PlayerUIWidget->AddToViewport();
	}
}

void ALestaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			InputSystem->AddMappingContext(InputMapping, InputMappingPriority);
		}

		InitializeLocalPlayerUI();
	}
}

void ALestaPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsValid(ControlledCharacter) && IsValid(PlayerUIWidget)) {
		PlayerUIWidget->SetHealthbarPercent(ControlledCharacter->GetHealth() / ControlledCharacter->GetMaxHealth());
	}
}

void ALestaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControlledCharacter = Cast<ALestaCharacter>(InPawn);
}
