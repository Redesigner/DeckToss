// Copyright (c) 2024 Stephen Melnick, Epic Games


#include "Ability/DeckAbilitySet.h"

#include "Logging/StructuredLog.h"

#include "Ability/DeckGameplayAbility.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "DeckGame.h"

void FDeckAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FDeckAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FDeckAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (Set)
	{
		GrantedAttributeSets.Add(Set);
	}
}

void FDeckAbilitySet_GrantedHandles::TakeFromAbilitySystem(UDeckAbilitySystemComponent* DeckAbilitySystemComponent)
{
	check(DeckAbilitySystemComponent);

	if (!DeckAbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			DeckAbilitySystemComponent->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			DeckAbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		DeckAbilitySystemComponent->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}

void UDeckAbilitySet::GiveToAbilitySystem(UDeckAbilitySystemComponent* DeckAbilitySystemComponent, FDeckAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(DeckAbilitySystemComponent);

	if (!DeckAbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		UE_LOGFMT(LogDeckGame, Warning, "Failed to give AbilitySet '{SetName}' to ASC. The owning actor does not have the authority to grant abilities.", GetName());
		// Must be authoritative to give or take ability sets.
		return;
	}
	
	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FDeckAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogDeckGame, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}
		UDeckGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UDeckGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = DeckAbilitySystemComponent->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FDeckAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogDeckGame, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(DeckAbilitySystemComponent->GetOwner(), SetToGrant.AttributeSet);
		DeckAbilitySystemComponent->AddAttributeSetSubobject(NewSet);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAttributeSet(NewSet);
		}
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FDeckAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogDeckGame, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = DeckAbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, DeckAbilitySystemComponent->MakeEffectContext());

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
}
