// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CardDeckInterface.generated.h"

class UCardDeckComponent;

UINTERFACE(MinimalAPI)
class UCardDeckInterface : public UInterface
{
	GENERATED_BODY()
};


class DECKGAME_API ICardDeckInterface
{
	GENERATED_BODY()

public:
	virtual UCardDeckComponent* GetCardDeckComponent() const = 0;
};
