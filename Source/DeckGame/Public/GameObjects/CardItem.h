// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "CardItem.generated.h"

UCLASS()
class DECKGAME_API ACardItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACardItem();

	FGameplayTag GetCardAbility() const;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess, Categories = "CardTag"))
	FGameplayTag CardAbility;
};
