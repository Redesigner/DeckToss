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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bActive = false;
};

UCLASS()
class DECKGAME_API UCardDeckComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCardDeckComponent();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCardsChanged, const TArray<FCardDeckEntry>&)

	void GiveCard(ACardItem* CardItem);

	const TArray<FCardDeckEntry>& GetCards() const;

	FOnCardsChanged OnCardsChanged;

	FGameplayAbilitySpecHandle GetAbilityBySlotIndex(uint8 Index) const;

	DECLARE_DELEGATE_OneParam(FOnCardAbilityPressed, FGameplayAbilitySpecHandle);
	FOnCardAbilityPressed OnCardAbilityPressed;

	void ChangeSelectedSlot(int IndexDelta);
	void ChangeSelectedCard(int IndexDelta);
	
	TMulticastDelegate<void(uint8)> OnSelectedSlotChanged;
	TMulticastDelegate<void(uint8)> OnSelectedCardChanged;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess, Categories = "CardTag"))
	TArray<FCardDeckEntry> Cards;

	uint8 CurrentlySelectedSlotIndex = 0;
	uint8 CurrentlySelectedCardIndex = 0;

	static TOptional<uint8> GetSlotIndex(FGameplayTag InputTag);
};
