// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "ThrowItemAbility.generated.h"

class ACardItem;
class UItemHandleComponent;

UCLASS()
class DECKGAME_API UThrowItemAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Throwing, meta = (AllowPrivateAccess))
	float MaxThrowSpeed = 600.0f;


	UThrowItemAbility();

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	void ActivateAbilityFailed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, int16 PredictionKey) override;

	void ActivateAbilitySucceed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FPredictionKey& PredictionKey) override;


	void ThrowItem(ACardItem* Item, APawn* Pawn, UItemHandleComponent* ItemHandle, float Strength);

	void SimulateThrowItem(ACardItem* Item, APawn* Pawn, UItemHandleComponent* ItemHandle, float Strength);

	void ServerThrowItem(ACardItem* Item, APawn* Pawn, UItemHandleComponent* ItemHandle, float Strength);
};
