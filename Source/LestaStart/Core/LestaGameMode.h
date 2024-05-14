// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "LestaCharacter/LestaCharacter.h"
#include "LestaGameMode.generated.h"

/** Base Game Mode class for the Lesta Start project. */
UCLASS()
class LESTASTART_API ALestaGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void ServerRegisterPlayer(ALestaCharacter* Player);

	UFUNCTION(Server, Reliable)
	void ServerHandlePlayerDeath(ALestaCharacter* Player);

protected:
	UPROPERTY()
	TArray<ALestaCharacter*> RegisteredPlayers;
	UPROPERTY()
	TArray<ALestaCharacter*> DeadPlayers;

	UFUNCTION()
	void CheckGameSessionEnd();
	UFUNCTION()
	void RestartGameSession();
};
