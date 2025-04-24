// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "CancelAbilitiesEffectComponent.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Cancel Gameplay Abilities")
class DECKGAME_API UCancelAbilitiesEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	void OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
	
#if WITH_EDITOR
	/** Warn on misconfigured Gameplay Effect */
	EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
};
