// Copyright (c) 2024 Stephen Melnick
#include "Ability/DeckAbilitySystemComponent.h"

#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Ability/DeckGameplayAbility.h"

UDeckAbilitySystemComponent::UDeckAbilitySystemComponent()
{
	UAbilitySystemComponent::SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void UDeckAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	if (InputHeldTags.Contains(InputTag))
	{
		return;
	}

	InputHeldTags.Add(InputTag);
	
	TArray<FGameplayAbilitySpecHandle> TagHandles;
	for (FGameplayAbilitySpec AbilitySpec : ActivatableAbilities.Items)
	{
		if (InputTag.MatchesAnyExact(AbilitySpec.GetDynamicSpecSourceTags()))
		{
			InsertSortPriority(TagHandles, AbilitySpec.Handle);
		}
	}

	// Sort the specs by priority before adding them to the 'stack'
	if (TagHandles.Num() > 0)
	{
		InputPressedSpecs.Add(TagHandles);
		UE_LOGFMT(LogDeckGame, Verbose, "DeckAbilitySystemComponent: Found '{Num}' Ability Specs matching InputTag '{Tag}'.", TagHandles.Num(), InputTag.GetTagName());
	}
}

void UDeckAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}

	for (FGameplayAbilitySpec AbilitySpec : ActivatableAbilities.Items)
	{
		if (InputTag.MatchesAnyExact(AbilitySpec.Ability->GetAssetTags()))
		{
			if (AbilitySpec.IsActive())
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}

	InputHeldTags.Remove(InputTag);
}

void UDeckAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		// This is necessary for the 'Wait Input Released' nodes to function properly
		InvokeReplicatedEvent(
			EAbilityGenericReplicatedEvent::InputReleased,
			Spec.Handle,
			Spec.GetPrimaryInstance() ? Spec.GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey() : FPredictionKey()
			);
	}
}

void UDeckAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<TArray<FGameplayAbilitySpecHandle>> AbilityBucketsToActivate;

	// Process all ability groups that had their input pressed this frame.
	// i.e. abilities grouped by InputTag
	//
	for (TArray<FGameplayAbilitySpecHandle> SpecBucket : InputPressedSpecs)
	{
		TArray<FGameplayAbilitySpecHandle> NewSpecBucket;
		for (FGameplayAbilitySpecHandle& AbilitySpecHandle : SpecBucket)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
			if (!AbilitySpec || !AbilitySpec->Ability)
			{
				continue;
			}

			if (AbilitySpec->IsActive())
			{
				// Ability is active so pass along the input event.
				UE_LOGFMT(LogDeckGame, Verbose, "DeckAbilitySystemComponent: Ability '{AbilityName}' input pressed.",
					AbilitySpec->Ability ? AbilitySpec->Ability->GetName() : "null");
				AbilitySpec->InputPressed = true;
				AbilitySpecInputPressed(*AbilitySpec);
			}
			else
			{
				const UDeckGameplayAbility* AbilityCDO = CastChecked<UDeckGameplayAbility>(AbilitySpec->Ability);

				if (AbilityCDO->GetActivationPolicy() == EAbilityActivationPolicy::OnInputPressed)
				{
					NewSpecBucket.Add(AbilitySpecHandle);
				}
			}
		}
		AbilityBucketsToActivate.Add(NewSpecBucket);
	}


	//
	// Do the same for our buckets. If one of the abilities in the buckets succeeds,
	// break out of the loop and don't bother trying to activate any of the other abilities.
	//
	for (TArray<FGameplayAbilitySpecHandle> AbilitySpecBucket : AbilityBucketsToActivate)
	{
		for (FGameplayAbilitySpecHandle AbilitySpecHandle : AbilitySpecBucket)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
			if (!AbilitySpec)
			{
				UE_LOGFMT(LogDeckGame, Error, "DeckAbilitySystemComponent: AbilitySpec was invalid.");
				continue;
			}

			if (!AbilitySpec->Ability)
			{
				UE_LOGFMT(LogDeckGame, Error, "DeckAbilitySystemComponent: AbilitySpec was valid, but the referenced ability pointer was invalid.");
				continue;
			}

			// Grab the ability before activating, in case the spec is removed
			UDeckGameplayAbility* DeckAbility = Cast<UDeckGameplayAbility>(AbilitySpec->Ability.Get());
			if (DeckAbility && AbilitySpec->InputPressed && DeckAbility->ConsumesInput())
			{
				UE_LOGFMT(LogDeckGame, Verbose, "DeckAbilitySystemComponent: Ability {AbilityName}, currently being held, consumed input.", DeckAbility->GetName());
				break;
			}
			// UE_LOGFMT(LogDeckGame, Display, "DeckAbilitySystemComponent: Trying to activate Ability {AbilityName}", DeckAbility->GetName());
			if (TryActivateAbility(AbilitySpec->Handle))
			{
				if (DeckAbility && DeckAbility->ConsumesInput())
				{
					UE_LOGFMT(LogDeckGame, Verbose, "DeckAbilitySystemComponent: Ability {AbilityName} consumed input.", DeckAbility->GetName());
					break;
				}
			}
		}
	}

	//
	// Process all abilities that had their input released this frame.
	//
	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				UE_LOGFMT(LogDeckGame, Verbose, "DeckAbilitySystemComponent: Ability '{AbilityName}' input released.",
					AbilitySpec->Ability ? AbilitySpec->Ability->GetName() : "null");
				AbilitySpec->InputPressed = false;

				if (AbilitySpec->IsActive())
				{
					// Ability is active so pass along the input event.
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	//
	// Clear the cached ability handles.
	//
	InputPressedSpecs.Reset();
	InputReleasedSpecHandles.Reset();
}

void UDeckAbilitySystemComponent::ClearAbilityInput()
{
}

bool UDeckAbilitySystemComponent::TryActivateAbilitiesByTag(FGameplayTag& Tag, bool bAllowRemoteActivation)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActivate;

	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (Tag.MatchesAnyExact(AbilitySpec.Ability->GetAssetTags()))
		{
			AbilitiesToActivate.AddUnique(AbilitySpec.Handle);
		}
	}
	bool bSuccess = false;
	
	for (FGameplayAbilitySpecHandle AbilitySpec : AbilitiesToActivate)
	{
		bSuccess |= TryActivateAbility(AbilitySpec, bAllowRemoteActivation);
	}

	return bSuccess;
}

bool UDeckAbilitySystemComponent::GetFirstAbilityForInputTag(const FGameplayTag& InputTag, FGameplayAbilitySpec& AbilitySpecOut) const
{
	TArray<FGameplayAbilitySpecHandle> Specs;
	for (FGameplayAbilitySpec AbilitySpec : ActivatableAbilities.Items)
	{
		if (InputTag.MatchesAnyExact(AbilitySpec.Ability->GetAssetTags()))
		{
			InsertSortPriority(Specs, AbilitySpec.Handle);
		}
	}

	for (FGameplayAbilitySpecHandle AbilitySpecHandle : Specs)
	{
		FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);

		if (!AbilitySpec)
		{
			continue;
		}

		if (UGameplayAbility* Ability = AbilitySpec->Ability)
		{
			if (Ability->CanActivateAbility(AbilitySpecHandle, AbilityActorInfo.Get()))
			{
				AbilitySpecOut = *AbilitySpec;
				return true;
			}
		}
	}
	return false;
}

void UDeckAbilitySystemComponent::RemoveAllActiveEffects()
{
	for (FActiveGameplayEffectHandle EffectHandle : GetActiveGameplayEffects().GetAllActiveEffectHandles())
	{
		RemoveActiveGameplayEffect(EffectHandle);
	}
}


void UDeckAbilitySystemComponent::InsertSortPriority(TArray<FGameplayAbilitySpecHandle>& Array, FGameplayAbilitySpecHandle SpecToInsert) const
{
	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecToInsert);
	if (!AbilitySpec)
	{
		UE_LOGFMT(LogDeckGame, Error, "DeckAbilitySystemComponent failed to sort ability by priority. AbilitySpecHandle was invalid.");
		return;
	}

	UDeckGameplayAbility* AbilityToInsert = Cast<UDeckGameplayAbility>(AbilitySpec->Ability);
	if (!AbilityToInsert)
	{
		UE_LOGFMT(LogDeckGame, Error, "DeckAbilitySystemComponent failed to sort ability by priority. Ability was not of type 'DeckGameplayAbility'.");
		return;
	}
	uint8 InsertedAbilityPriority = AbilityToInsert->GetPriority();

	int i = 0;
	for (; i < Array.Num(); ++i)
	{
		FGameplayAbilitySpec* ExistingSpec = FindAbilitySpecFromHandle(Array[i]);
		if (!ExistingSpec)
		{
			continue;
		}
		UDeckGameplayAbility* ExistingAbility = Cast<UDeckGameplayAbility>(ExistingSpec->Ability);
		if (!ExistingAbility)
		{
			continue;
		}
		if (InsertedAbilityPriority >= ExistingAbility->GetPriority())
		{
			break;
		}
	}

	Array.Insert(SpecToInsert, i);
}

void UDeckAbilitySystemComponent::ClientActivateAbilityFailed_Implementation(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey)
{
	Super::ClientActivateAbilityFailed_Implementation(AbilityToActivate, PredictionKey);

	FGameplayAbilitySpec* FailedAbilitySpec = FindAbilitySpecFromHandle(AbilityToActivate);
	if (!FailedAbilitySpec)
	{
		return;
	}
	
	if (UDeckGameplayAbility* FailedAbility = Cast<UDeckGameplayAbility>(FailedAbilitySpec->Ability))
	{
		const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
		FailedAbility->ActivateAbilityFailed(
			AbilityToActivate,
			ActorInfo,
			FailedAbilitySpec->GetPrimaryInstance() ? FailedAbilitySpec->GetPrimaryInstance()->GetCurrentActivationInfo() : FGameplayAbilityActivationInfo(),
			PredictionKey
		);
	}
}

void UDeckAbilitySystemComponent::ClientActivateAbilitySucceedWithEventData_Implementation(FGameplayAbilitySpecHandle Handle, FPredictionKey PredictionKey, FGameplayEventData TriggerEventData)
{
	Super::ClientActivateAbilitySucceedWithEventData_Implementation(Handle, PredictionKey, TriggerEventData);

	FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(Handle);
	if (!AbilitySpec)
	{
		return;
	}

	if (UDeckGameplayAbility* FailedAbility = Cast<UDeckGameplayAbility>(AbilitySpec->Ability))
	{
		const FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
		FailedAbility->ActivateAbilitySucceed(
			Handle,
			ActorInfo,
			AbilitySpec->GetPrimaryInstance() ? AbilitySpec->GetPrimaryInstance()->GetCurrentActivationInfo() : FGameplayAbilityActivationInfo(),
			PredictionKey
		);
	}
}

void UDeckAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	OnAbilitiesChanged.Broadcast();
}
