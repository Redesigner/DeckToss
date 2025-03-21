// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "SelectItemAbility.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API USelectItemAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()

	USelectItemAbility();

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
