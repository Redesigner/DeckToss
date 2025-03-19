// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Abilities/StashItemAbility.h"

#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Character/Components/CardDeckComponent.h"
#include "Character/Components/CardDeckInterface.h"
#include "Character/Components/ItemHandleComponent.h"
#include "GameObjects/CardItem.h"

void UStashItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	APawn* Owner = Cast<APawn>(ActorInfo->AvatarActor);
	if (!Owner)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ICardDeckInterface* CardDeckInterface = Cast<ICardDeckInterface>(Owner);
	if (!CardDeckInterface)
	{
		UE_LOGFMT(LogDeckGame, Warning, "StashItemAbility: Ability failed to activate. Owning actor '{}' must implement ICardDeckInterface.", Owner->GetName());
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	UCardDeckComponent* CardDeck = CardDeckInterface->GetCardDeckComponent();
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
	HeldItem->Destroy();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
