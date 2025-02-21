// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#include "ViewModel/CardDeckViewModel.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

#include "DeckUI.h"
#include "Character/DeckPlayerState.h"
#include "Game/DeckGameMode.h"

void UCardDeckViewModel::BindPlayerState(ADeckPlayerState* PlayerState)
{
	UCardDeckComponent* CardDeck = PlayerState->GetCardDeckComponent();
	if (!CardDeck)
	{
		UE_LOGFMT(LogDeckUI, Warning, "CardDeckViewModel: failed to bind to player state. PlayerState '{}' did not have a valid Card Deck.", PlayerState ? PlayerState->GetName() : "null");
		return;
	}

	CardDeck->OnCardsChanged.AddUObject(this, &ThisClass::CardsChanged);
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
		NewEntry->Entry.CardName = FText::FromString(Card.CardTag.ToString());
		CardData.Add(NewEntry);
	}
	BroadcastFieldValueChanged(ThisClass::FFieldNotificationClassDescriptor::CardData);
}
