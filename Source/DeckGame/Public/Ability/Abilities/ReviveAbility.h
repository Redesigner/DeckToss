// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "ReviveAbility.generated.h"


class ADeckPlayerState;

UCLASS()
class DECKGAME_API UReviveAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()
	
	/// The effect to be applied to the player while they are revivng someone
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> ReviveEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float MaxTimeBetweenPresses = 1.0f;
	
	UReviveAbility();
	
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	FActiveGameplayEffectHandle AppliedReviveEffect;

	FTimerHandle ButtonPressTimer;

	FDelegateHandle TargetRevivedDelegateHandle;
	
	TWeakObjectPtr<ADeckPlayerState> RevivalTarget;
};
