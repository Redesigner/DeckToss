// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#include "ViewModel/CardDeckViewModel.h"

#include "Logging/StructuredLog.h"

#include "DeckUI.h"
#include "Character/DeckPlayerState.h"

void UCardDeckViewModel::BindPlayerState(ADeckPlayerState* PlayerState)
{
	UCardDeckComponent* CardDeck = PlayerState->GetCardDeckComponent();
	if (!CardDeck)
	{
		UE_LOGFMT(LogDeckUI, Warning, "CardDeckViewModel: failed to bind to player state. PlayerState '{}' did not have a valid Card Deck.", PlayerState ? PlayerState->GetName() : "null");
		return;
	}

	CardDeck->OnCardsChanged.AddUObject(this, &ThisClass::CardsChanged);
	CardDeck->OnSelectedCardChanged.AddUObject(this, &ThisClass::SelectedCardChanged);
	CardDeck->OnSelectedSlotChanged.AddUObject(this, &ThisClass::SlotChanged);
	CardsChanged(CardDeck->GetCards());
}

void UCardDeckViewModel::CardsChanged(const TArray<FCardDeckEntry>& Cards)
{
	//ADeckGameMode* DeckGameMode = Cast<ADeckGameMode>(UGameplayStatics::GetGameMode(this));

	//if (!DeckGameMode)
	//{
	//	UE_LOGFMT(LogDeckUI, Warning, "CardDeckViewModel: failed to update viewmodel. The gamemode was not of type ADeckGameMode.");
	//	return;
	//}

	CardData.Empty();

	for (const FCardDeckEntry& Card : Cards)
	{
		UCardDeckUIData* NewEntry = NewObject<UCardDeckUIData>();
		FString CardName = Card.CardTag.ToString();
		int32 Separator = 0;
		if (CardName.FindChar('.', Separator))
		{
			CardName = CardName.RightChop(Separator + 1);
		}
		NewEntry->Entry.CardName = FText::FromString(CardName);
		CardData.Add(NewEntry);
	}
	BroadcastFieldValueChanged(ThisClass::FFieldNotificationClassDescriptor::CardData);
}

void UCardDeckViewModel::SlotChanged(uint8 SlotIndex)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectedSlot, SlotIndex);
}

void UCardDeckViewModel::SelectedCardChanged(uint8 CardIndex)
{
	UE_MVVM_SET_PROPERTY_VALUE(SelectedCard, CardIndex);
}
