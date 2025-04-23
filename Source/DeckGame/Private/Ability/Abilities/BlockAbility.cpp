// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Abilities/BlockAbility.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/Components/MeleeComponent.h"

void UBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo->AvatarActor.IsValid())
	{
		ABILITY_CANCEL_AND_RETURN();
	}
	
	ABILITY_GET_COMPONENT_OR_CANCEL(UMeleeComponent, MeleeComponent, ActorInfo->AvatarActor);

	if (!BlockAnimation)
	{
		ABILITY_CANCEL_AND_RETURN();
	}

	// ActorInfo->AnimInstance->Pla
	UAbilityTask_PlayMontageAndWait* BlockingAnimationTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, "Play blocking animation", BlockAnimation);
	BlockingAnimationTask->ReadyForActivation();

	for (FMeleeHitboxSpawnParameters& Hitbox : Hitboxes)
	{
		MeleeComponent->SpawnHitbox(Hitbox);
	}
}

void UBlockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ActorInfo->AvatarActor.IsValid())
	{
		if (UMeleeComponent* MeleeComponent = ActorInfo->AvatarActor->GetComponentByClass<UMeleeComponent>())
		{
			MeleeComponent->DestroyAllHitboxes();
		}
	}
}

void UBlockAbility::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
