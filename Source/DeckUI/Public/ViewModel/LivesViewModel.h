// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "LivesViewModel.generated.h"

class ADeckPlayerState;

UCLASS()
class DECKUI_API ULivesViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, FieldNotify, Meta = (AllowPrivateAccess))
	uint8 LivesCount;

public:
	UFUNCTION(BlueprintCallable)
	void BindPlayerState(ADeckPlayerState* DeckPlayerState);

	void LivesCountChanged(uint8 NewCount);
};
