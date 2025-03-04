﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LestaCharacter.h"
#include "GameFramework/PlayerController.h"
#include "LestaStart/UI/PlayerUIWidget.h"
#include "LestaPlayerController.generated.h"

class UInputMappingContext;

/** Base Player Controller class for the Lesta Start project. */
UCLASS()
class LESTASTART_API ALestaPlayerController : public APlayerController
{
	GENERATED_BODY()

	// You can extend this class if required
	// Blueprint PlayerController class is derived from the ALestaPlayerController

protected:
	/** Added input mapping context on startup. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	
	/** Priority of InputMapping. */
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (ClampMin = 0))
	int32 InputMappingPriority = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerUIWidget> PlayerUIWidgetClass;
	UPROPERTY()
	TObjectPtr<UPlayerUIWidget> PlayerUIWidget;
	
	void InitializeLocalPlayerUI();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
