// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "NativeGameplayTags.h"

#include "DeckAbilitySystemComponent.generated.h"

UCLASS()
class DECKGAME_API UDeckAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnAbilitiesChanged)

public:
	UDeckAbilitySystemComponent();

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	void ClearAbilityInput();

	bool TryActivateAbilitiesByTag(FGameplayTag& Tag, bool bAllowRemoteActivation = true);

	bool GetFirstAbilityForInputTag(const FGameplayTag& InputTag, FGameplayAbilitySpec& AbilitySpecOut) const;

	void RemoveAllActiveEffects();

	FOnAbilitiesChanged OnAbilitiesChanged;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bCanAnimationBeCancelled = false;

protected:
	// Abilities grouped together by InputTags
	TArray<TArray<FGameplayAbilitySpecHandle>> InputPressedSpecs;

	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayTag> InputHeldTags;

private:
	void InsertSortPriority(TArray<FGameplayAbilitySpecHandle>& Array, FGameplayAbilitySpecHandle SpecToInsert) const;

	void ClientActivateAbilityFailed_Implementation(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey) override;
	void ClientActivateAbilitySucceedWithEventData_Implementation(FGameplayAbilitySpecHandle Handle, FPredictionKey PredictionKey, FGameplayEventData TriggerEventData) override;
	void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
};