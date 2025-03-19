// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Effects/LaunchGameplayEffectComponent.h"

#include "AbilitySystemComponent.h"
#include "Ability/Effects/GameplayEffectContainer.h"
#include "GameFramework/Character.h"

void ULaunchGameplayEffectComponent::OnGameplayEffectExecuted(FActiveGameplayEffectsContainer& ActiveGEContainer,
                                                                    FGameplayEffectSpec& GESpec, FPredictionKey& PredictionKey) const
{
	Super::OnGameplayEffectExecuted(ActiveGEContainer, GESpec, PredictionKey);

	if (ActiveGEContainer.Owner)
	{
		FVector LaunchVector = Displacement * ScalingFactor;
		if (GESpec.GetEffectContext().GetHitResult())
		{
			LaunchVector = GESpec.GetEffectContext().GetHitResult()->Normal.Rotation().RotateVector(LaunchVector);
		}
		if (ACharacter* Character = Cast<ACharacter>(ActiveGEContainer.Owner->GetAvatarActor()))
		{
			Character->LaunchCharacter(LaunchVector, false, false);
		}
	}
}
