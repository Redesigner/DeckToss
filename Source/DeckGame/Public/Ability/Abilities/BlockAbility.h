// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "Character/Components/MeleeComponent.h"
#include "BlockAbility.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API UBlockAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = Blocking, Meta = (AllowPrivateAccess))
	TArray<FMeleeHitboxSpawnParameters> Hitboxes;

	// Animation that plays when the character starts blocking
	// It should loop if the block is a continuous state
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blocking|Animation", Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> BlockAnimation;

	// Animation to play when a hit is blocked
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Blocking|Animation", Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> BlockHitAnimation;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
};
