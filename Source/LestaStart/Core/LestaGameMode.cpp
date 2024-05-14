
#include "LestaGameMode.h"

void ALestaGameMode::ServerRegisterPlayer_Implementation(ALestaCharacter* Player)
{
	if (!RegisteredPlayers.Contains(Player))
		RegisteredPlayers.Add(Player);
}

void ALestaGameMode::ServerHandlePlayerDeath_Implementation(ALestaCharacter* Player)
{
	if (!DeadPlayers.Contains(Player))
		DeadPlayers.Add(Player);
	
	CheckGameSessionEnd();
}

void ALestaGameMode::CheckGameSessionEnd()
{
	bool IsEnded = true;
	for (ALestaCharacter* RegisteredPlayer : RegisteredPlayers)
	{
		if (!DeadPlayers.Contains(RegisteredPlayer))
		{
			IsEnded = false;
			break;
		}
	}

	if (IsEnded)
	{
		RestartGameSession();
	}
}

void ALestaGameMode::RestartGameSession()
{
	RestartGame();
	for (ALestaCharacter* RegisteredPlayer : RegisteredPlayers)
	{
		AController* PlayerController = RegisteredPlayer->GetController();
		RestartPlayer(PlayerController);
	}
	DeadPlayers.Empty();
}
