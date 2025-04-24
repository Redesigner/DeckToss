// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Effects/CancelAbilitiesEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Misc/DataValidation.h"

#define LOCTEXT_NAMESPACE "CancelAbilitiesEffectComponent"

void UCancelAbilitiesEffectComponent::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer,
	FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectExecuted(ActiveGEContainer, GESpec, PredictionKey);
	
	if (ActiveGEContainer.Owner)
	{
		ActiveGEContainer.Owner->CancelAllAbilities();
	}
}

#if WITH_EDITOR
EDataValidationResult UCancelAbilitiesEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (GetOwner()->DurationPolicy != EGameplayEffectDurationType::Instant)
	{
		Context.AddError(FText::FormatOrdered(LOCTEXT("CancelAbilitiesRequireInstant", "CancelAbilitiesEffects require effects with Instant duration: {0}."), FText::FromString(GetClass()->GetName())));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}
#endif

#undef LOCTEXT_NAMESPACE