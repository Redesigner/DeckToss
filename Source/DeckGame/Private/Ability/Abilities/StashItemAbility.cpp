// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Abilities/StashItemAbility.h"

#include "Character/Components/CardDeckComponent.h"
#include "Character/Components/ItemHandleComponent.h"

void UStashItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	APawn* Owner = Cast<APawn>(ActorInfo->AvatarActor);
	if (!Owner)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	UCardDeckComponent* CardDeck = Owner->GetComponentByClass<UCardDeckComponent>();
	if (!CardDeck)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	UItemHandleComponent* ItemHandle = Owner->GetComponentByClass<UItemHandleComponent>();
	if (!ItemHandle)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ACardItem* HeldItem = ItemHandle->ConsumeItem();
	if (!HeldItem)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	CardDeck->GiveCard(HeldItem);
}
