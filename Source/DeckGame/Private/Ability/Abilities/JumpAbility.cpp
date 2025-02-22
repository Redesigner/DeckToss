// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Abilities/JumpAbility.h"

#include "Character/PlayerCharacter/PlayerCharacter.h"

bool UJumpAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const APlayerCharacter* DeckCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!DeckCharacter || !DeckCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	APlayerCharacter* DeckCharacter = Cast<APlayerCharacter>(ActorInfo->AvatarActor.Get());
	if (!DeckCharacter || !DeckCharacter->CanJump())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	DeckCharacter->Jump();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
