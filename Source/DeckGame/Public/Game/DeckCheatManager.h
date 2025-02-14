// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DeckCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API UDeckCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
	UFUNCTION(Exec)
	void SetImmortal(bool bImmortal);

	/// Kill the currently controlled player
	UFUNCTION(Exec)
	void KillPlayer();
};
