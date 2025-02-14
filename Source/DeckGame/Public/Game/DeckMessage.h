// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"

#include "DeckMessage.generated.h"

USTRUCT(BlueprintType)
struct DECKGAME_API FDeckMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTag Verb;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<AActor> Source;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTag Data;
};