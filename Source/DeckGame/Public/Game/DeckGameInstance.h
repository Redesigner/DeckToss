// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Engine/StreamableManager.h" 

#include "DeckGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API UDeckGameInstance : public UGameInstance
{
	GENERATED_BODY()

	FStreamableManager StreamableManager;

public:
	FStreamableManager& GetStreamableManager();
};
