// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CardAbilityMap.generated.h"

class ACardItem;
class UDeckGameplayAbility;

USTRUCT(BlueprintType)
struct DECKGAME_API FCardAbilityEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "CardTag"))
	FGameplayTag CardTag;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDeckGameplayAbility> CardAbility;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACardItem> CardItem;
};

UCLASS()
class DECKGAME_API UCardAbilityMap : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "CardTag"))
	TArray<FCardAbilityEntry> Cards;

	bool FindAbilityFromTag(FGameplayTag CardTag, UDeckGameplayAbility*& AbilityOut) const;

	bool FindItemFromTag(FGameplayTag CardTag, TSubclassOf<ACardItem>& ItemOut) const;
};
