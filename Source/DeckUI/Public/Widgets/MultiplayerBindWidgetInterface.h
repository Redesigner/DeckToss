// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MultiplayerBindWidgetInterface.generated.h"

class ADeckPlayerState;

UINTERFACE(MinimalAPI)
class UMultiplayerBindWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

class DECKUI_API IMultiplayerBindWidgetInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BindPlayer(ADeckPlayerState* PlayerState);
};
