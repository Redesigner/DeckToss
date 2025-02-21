// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "Character/Components/CardDeckComponent.h"

#include "CardDeckViewModel.generated.h"

class ADeckPlayerState;

USTRUCT(BlueprintType)
struct DECKUI_API FCardDeckUIEntry
{
	GENERATED_BODY()

	FText CardName;
};

/// Object wrapper for use in UMG
UCLASS(BlueprintType)
class DECKUI_API UCardDeckUIData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FCardDeckUIEntry Entry;
};

UCLASS()
class UCardDeckViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BindPlayerState(ADeckPlayerState* PlayerState);

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UCardDeckUIData>> CardData;

	void CardsChanged(const TArray<FCardDeckEntry>& Cards);
};