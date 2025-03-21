// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Abilities/SelectItemAbility.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Ability/DeckGameplayTags.h"
#include "Character/Components/CardDeckComponent.h"
#include "Character/Components/ItemHandleComponent.h"
#include "Game/CardAbilityMap.h"
#include "Game/DeckGameMode.h"

USelectItemAbility::USelectItemAbility()
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = DeckGameplayTags::GameplayEvent_AbilityTrigger_SelectSlot;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void USelectItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ABILITY_GET_OBJECT_OR_CANCEL(ADeckGameMode*, GameMode, Cast<ADeckGameMode>(UGameplayStatics::GetGameMode(this)));
	ABILITY_GET_OBJECT_OR_CANCEL(UCardAbilityMap*, CardAbilityMap, GameMode->GetCards());
	
	if (!ActorInfo->AvatarActor.IsValid() || !ActorInfo->OwnerActor.IsValid())
	{
		ABILITY_CANCEL_AND_RETURN();
	}

	ABILITY_GET_COMPONENT_OR_CANCEL(UCardDeckComponent, CardDeckComponent, ActorInfo->OwnerActor);
	ABILITY_GET_COMPONENT_OR_CANCEL(UItemHandleComponent, ItemHandleComponent, ActorInfo->AvatarActor)

	if (ItemHandleComponent->HoldingItem())
	{
		ABILITY_CANCEL_AND_RETURN();
	}

	uint8 HeldItemIndex = static_cast<uint8>(TriggerEventData->EventMagnitude);
	TOptional<FCardDeckEntry> Card = CardDeckComponent->ConsumeCard(HeldItemIndex);
	if (!Card)
	{
		ABILITY_CANCEL_AND_RETURN();
	}

	TSubclassOf<ACardItem> CardItemClass;
	if (!CardAbilityMap->FindItemFromTag(Card.GetValue().CardTag, CardItemClass))
	{
		UE_LOGFMT(LogDeckGame, Warning, "SelectItemAbility: could not find a valid item matching '{CardTag}' in CardAbilityMap '{AbilityMap}",
			Card.GetValue().CardTag.ToString(), CardAbilityMap->GetName());
		ABILITY_CANCEL_AND_RETURN();
	}

	ACardItem* PickedUpItem = GetWorld()->SpawnActorDeferred<ACardItem>(CardItemClass, ActorInfo->AvatarActor->GetTransform());
	// Set held early, to make sure it isn't collected by any volumes
	PickedUpItem->bHeld = true;
	PickedUpItem->FinishSpawning(ActorInfo->AvatarActor->GetTransform());
	ItemHandleComponent->PickupItem(PickedUpItem);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
