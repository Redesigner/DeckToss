// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "GameObjects/CardItem.h"

ACardItem::ACardItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

FGameplayTag ACardItem::GetCardAbility() const
{
	return CardAbility;
}
