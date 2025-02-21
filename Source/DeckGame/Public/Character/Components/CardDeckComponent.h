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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FGameplayTag CardTag;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FGameplayAbilitySpecHandle GrantedAbility;
};

UCLASS()
class DECKGAME_API UCardDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCardDeckComponent();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCardsChanged, const TArray<FCardDeckEntry>&)

	void GiveCard(ACardItem* CardItem);

	const TArray<FCardDeckEntry> GetCards() const;

	FOnCardsChanged OnCardsChanged;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess, Categories = "CardTag"))
	TArray<FCardDeckEntry> Cards;
};
