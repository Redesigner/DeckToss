// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponent.h"
#include "LaunchGameplayEffectComponent.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API ULaunchGameplayEffectComponent : public UGameplayEffectComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Displacement, Meta = (AllowPrivateAccess))
	FVector Displacement;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Displacement, Meta = (AllowPrivateAccess, ClampMini = 0.0f))
	float ScalingFactor = 1.0f;

	void OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer, FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const override;
};
