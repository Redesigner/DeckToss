// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Widgets/MultiplayerBindWidgetInterface.h"
#include "MultiHudWidget.generated.h"

class ADeckPlayerState;

UCLASS()
class DECKUI_API UMultiHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UNamedSlot> PlayerSlot1;

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UNamedSlot> PlayerSlot2;	

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UNamedSlot> PlayerSlot3;	

	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<UNamedSlot> PlayerSlot4;


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess, MustImplement="MultiplayerBindWidgetInterface"))
	TSubclassOf<UUserWidget> HudClass;

	void NativeConstruct() override;

	void ConstructPlayerHud(ADeckPlayerState* PlayerState, UNamedSlot* NamedSlot);
};
