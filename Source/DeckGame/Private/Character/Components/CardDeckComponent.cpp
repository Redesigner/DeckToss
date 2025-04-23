// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Character/Components/CardDeckComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Ability/DeckGameplayAbility.h"
#include "Ability/DeckGameplayTags.h"
#include "Game/CardAbilityMap.h"
#include "Game/DeckGameMode.h"
#include "GameObjects/CardItem.h"

UCardDeckComponent::UCardDeckComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (int i = 0; i < 3; ++i)
	{
		Cards.Add({FGameplayTag(), FGameplayAbilitySpecHandle()});
	}
}

void UCardDeckComponent::GiveCard(ACardItem* CardItem)
{
	IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(GetOwner());
	if (!AbilitySystem)
	{
		UE_LOGFMT(LogDeckGame, Warning, "CardDeck Component: could not grant card ability. The owner of the component must implement IAbilitySystemInterface.");
		return;
	}

	UAbilitySystemComponent* ASC = AbilitySystem->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOGFMT(LogDeckGame, Warning, "CardDeck Component: could not grant card ability. The owner of the component must have a valid AbilitySystemComponent.");
		return;
	}

	ADeckGameMode* CurrentGameMode = Cast<ADeckGameMode>(UGameplayStatics::GetGameMode(this));
	if (!CurrentGameMode)
	{
		UE_LOGFMT(LogDeckGame, Warning, "CardDeck Component: could not grant card ability. The game mode was not a DeckGameMode.");
		return;
	}

	UCardAbilityMap* CardMap = CurrentGameMode->GetCards();
	if (!CardMap)
	{
		UE_LOGFMT(LogDeckGame, Warning, "CardDeck Component: could not grant card ability. The game mode did not have a card ability map.");
		return;
	}

	FCardAbilityEntry CardEntry;
	if (!CardMap->FindEntryFromItem(CardItem->GetClass(), CardEntry))
	{
		UE_LOGFMT(LogDeckGame, Warning, "CardDeck Component: could not grant card ability. Could not find an ability for item {}", CardItem ? CardItem->GetName() : "null");
		return;
	}

	FGameplayAbilitySpec NewSpec = FGameplayAbilitySpec(CardEntry.CardAbility->GetDefaultObject<UDeckGameplayAbility>(), 1);
	// NewSpec.GetDynamicSpecSourceTags().AddTag(DeckGameplayTags::InputTag_Attack);
	FGameplayAbilitySpecHandle NewAbilityHandle = ASC->GiveAbility(NewSpec);

	bool bEmptySlot = false;
	for (int i = 0; i < 3; ++i)
	{
		if (!Cards[i].CardTag.IsValid())
		{
			Cards[i] = FCardDeckEntry(CardEntry.CardTag, NewAbilityHandle);
			bEmptySlot = true;
			break;
		}
	}

	if (!bEmptySlot)
	{
		Cards.Add(FCardDeckEntry(CardEntry.CardTag, NewAbilityHandle));
	}

	OnCardsChanged.Broadcast(Cards);
}

const TArray<FCardDeckEntry>& UCardDeckComponent::GetCards() const
{
	return Cards;
}

TOptional<FCardDeckEntry> UCardDeckComponent::GetCard(uint8 Index) const
{
	if (Index >= Cards.Num() || Cards[Index].CardTag.IsValid())
	{
		return TOptional<FCardDeckEntry>();
	}

	return Cards[Index];
}

TOptional<FCardDeckEntry> UCardDeckComponent::ConsumeCard(uint8 Index)
{
	if (Index >= Cards.Num() || !Cards[Index].CardTag.IsValid())
	{
		return TOptional<FCardDeckEntry>();
	}
	FCardDeckEntry RemovedCard = Cards[Index];
	Cards[Index].CardTag = FGameplayTag();
	Cards[Index].GrantedAbility = FGameplayAbilitySpecHandle();
	if (IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		if (UAbilitySystemComponent* ASC = AbilitySystem->GetAbilitySystemComponent())
		{
			ASC->ClearAbility(RemovedCard.GrantedAbility);
		}
	}

	OnCardsChanged.Broadcast(Cards);
	return RemovedCard;
}

FGameplayAbilitySpecHandle UCardDeckComponent::GetAbilityBySlotIndex(uint8 Index) const
{
	if (Index >= Cards.Num())
	{
		return FGameplayAbilitySpecHandle();
	}

	return Cards[Index].GrantedAbility;
}

void UCardDeckComponent::ChangeSelectedSlot(int IndexDelta)
{
	// The currently selected slot index can be one past the end, in case a player wants to move an ability to a new slot
	int Index = FMath::Modulo(CurrentlySelectedSlotIndex + IndexDelta, Cards.Num() + 1);
	CurrentlySelectedSlotIndex = Index < 0 ? Cards.Num() + Index : Index;
	OnSelectedSlotChanged.Broadcast(CurrentlySelectedSlotIndex);
}

void UCardDeckComponent::ChangeSelectedCard(int IndexDelta)
{
	int Index = FMath::Modulo(CurrentlySelectedCardIndex + IndexDelta, Cards.Num());
	CurrentlySelectedCardIndex = Index < 0 ? Cards.Num() + Index : Index;
	OnSelectedCardChanged.Broadcast(CurrentlySelectedCardIndex);
}

void UCardDeckComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	TOptional<uint8> CardSlot = GetSlotIndex(InputTag);
	if (CardSlot && CardSlot.GetValue() < Cards.Num())
	{
		if (Cards[CardSlot.GetValue()].bActive || !Cards[CardSlot.GetValue()].GrantedAbility.IsValid())
		{
			return;
		}
		Cards[CardSlot.GetValue()].bActive = true;
		OnCardAbilityPressed.ExecuteIfBound(Cards[CardSlot.GetValue()].GrantedAbility);
	}
}

void UCardDeckComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	TOptional<uint8> CardSlot = GetSlotIndex(InputTag);
	if (CardSlot && CardSlot.GetValue() < Cards.Num())
	{
		Cards[CardSlot.GetValue()].bActive = false;
		OnCardAbilityReleased.ExecuteIfBound(Cards[CardSlot.GetValue()].GrantedAbility);
	}
}

void UCardDeckComponent::SetSelectionMode(bool bSelectionModeIn)
{
	if (bSelectionModeIn == bSelectionMode)
	{
		return;
	}

	bSelectionMode = bSelectionModeIn;
	OnSelectionModeChanged.Broadcast(bSelectionMode);
}

TOptional<uint8> UCardDeckComponent::GetSlotIndex(FGameplayTag InputTag)
{
	if (InputTag == DeckGameplayTags::InputTag_Slot_0)
	{
		return 0;
	}

	if (InputTag == DeckGameplayTags::InputTag_Slot_1)
	{
		return 1;
	}

	if (InputTag == DeckGameplayTags::InputTag_Slot_2)
	{
		return 2;
	}

	return TOptional<uint8>();
}

void UCardDeckComponent::DeactivateCards()
{
	for (FCardDeckEntry& Card : Cards)
	{
		Card.bActive = false;
	}
}