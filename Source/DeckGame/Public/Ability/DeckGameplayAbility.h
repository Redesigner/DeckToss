// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "DeckGameplayAbility.generated.h"


class UBlackboardComponent;

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	OnInputPressed,
	Repeated,
	OnSpawn
};

UCLASS()
class DECKGAME_API UDeckGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AbilityActivation, meta = (AllowPrivateAccess = true))
	uint8 Priority = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AbilityActivation, meta = (AllowPrivateAccess = true))
	bool bConsumeInput = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = AbilityActivation, meta = (AllowPrivateAccess = true))
	FText TipName;

public:
	EAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	uint8 GetPriority() const { return Priority; }

	bool ConsumesInput() const { return bConsumeInput; }

	FText GetTipName() const { return TipName; }

	virtual void ActivateAbilityFailed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, int16 PredictionKey);

	virtual void ActivateAbilitySucceed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FPredictionKey& PredictionKey);

	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Activation")
	EAbilityActivationPolicy ActivationPolicy;

	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = bBlackboardFound))
	UBlackboardComponent* GetOwnerBlackboard(bool& bBlackboardFound);
};
