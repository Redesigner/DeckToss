// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "CardDeckComponent.generated.h"

class ACardItem;

USTRUCT(BlueprintType)
struct DECKGAME_API FCardDeckEntry
{
	GENERATED_BODY()

	FGameplayTag CardTag;
	FGameplayAbilitySpecHandle GrantedAbility;
};

UCLASS()
class DECKGAME_API UCardDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCardDeckComponent();

	void GiveCard(ACardItem* CardItem);

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (AllowPrivateAccess, Categories = "CardTag"))
	TArray<FCardDeckEntry> Cards;
};
