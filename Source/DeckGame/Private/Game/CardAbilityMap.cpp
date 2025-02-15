// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Game/CardAbilityMap.h"

bool UCardAbilityMap::FindAbilityFromTag(FGameplayTag CardTag, UDeckGameplayAbility*& AbilityOut) const
{
	for (const FCardAbilityEntry& Card : Cards)
	{
		if (Card.CardTag == CardTag)
		{
			AbilityOut = Card.CardAbility;
			return true;
		}
	}
	return false;
}

bool UCardAbilityMap::FindItemFromTag(FGameplayTag CardTag, TSubclassOf<ACardItem>& ItemOut) const
{
	for (const FCardAbilityEntry& Card : Cards)
	{
		if (Card.CardTag == CardTag)
		{
			ItemOut = Card.CardItem;
			return true;
		}
	}
	return false;
}
