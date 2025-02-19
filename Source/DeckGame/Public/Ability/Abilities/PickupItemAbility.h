// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "PickupItemAbility.generated.h"

class ACardItem;
class UItemHandleComponent;

UCLASS()
class DECKGAME_API UPickupItemAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()

	UPickupItemAbility();
	
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	void ActivateAbilityFailed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, int16 PredictionKey) override;

	void ActivateAbilitySucceed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FPredictionKey& PredictionKey) override;

	
	void PickupItem(ACardItem* Item, UItemHandleComponent* ItemHandle);

	void SimulatePickupItem(ACardItem* Item, UItemHandleComponent* ItemHandle);

	void ServerPickupItem(ACardItem* Item, UItemHandleComponent* ItemHandle);
};
