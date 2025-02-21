// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GameplayTagContainer.h"

#include "DeckLayoutMap.generated.h"

class UCommonActivatableWidget;

USTRUCT(BlueprintType)
struct DECKUI_API FDeckLayoutMapEntry
{
	GENERATED_BODY()

	// The tag received with the message
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "UI.MessageTag"))
	FGameplayTag MessageTag;

	// The slot to put this widget into
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "UI.SlotTag"))
	FGameplayTag SlotTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> Widget;
};

UCLASS()
class DECKUI_API UDeckLayoutMap : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FDeckLayoutMapEntry> LayoutEntries;

public:	
	bool FindWidgetLayoutForMessage(FGameplayTag MessageTag, FDeckLayoutMapEntry& WidgetLayoutOut) const;
};
