// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CheatManager.h"
#include "DeckCheatManager.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DECKGAME_API UDeckCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
	UFUNCTION(Exec)
	void SetImmortal(bool bImmortal);
	
	UFUNCTION(Exec)
	void SetInvulnerable(bool bInvuln);

	/// Kill the currently controlled player
	UFUNCTION(Exec)
	void KillPlayer();

	void ApplyTag(UAbilitySystemComponent* ASC, FGameplayTag Tag, bool bApply);
};
