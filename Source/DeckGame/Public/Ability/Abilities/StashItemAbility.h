// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "StashItemAbility.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API UStashItemAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()
	
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)  override;
};
