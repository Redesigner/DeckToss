// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Character/Components/CardDeckComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Ability/DeckGameplayAbility.h"
#include "Game/CardAbilityMap.h"
#include "Game/DeckGameMode.h"
#include "GameObjects/CardItem.h"

UCardDeckComponent::UCardDeckComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
	FGameplayAbilitySpecHandle NewAbilityHandle = ASC->GiveAbility(NewSpec);
	Cards.Add(FCardDeckEntry(CardEntry.CardTag, NewAbilityHandle));
}
