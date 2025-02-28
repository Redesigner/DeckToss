// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Widgets/MultiHudWidget.h"

#include "Components/NamedSlot.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

#include "DeckUI.h"
#include "Character/DeckPlayerState.h"

void UMultiHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// As with the camera, don't create these widgets if there is no viewport
	if (!GetOwningLocalPlayer() || !GetOwningLocalPlayer()->ViewportClient)
	{
		return;
	}

	if (!HudClass)
	{
		UE_LOGFMT(LogDeckUI, Error, "MultiHudWidget: Failed to construct widgets. HudClass was invalid. Did you forget to set it?");
		return;
	}

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
	if (!GameState)
	{
		UE_LOGFMT(LogDeckUI, Error, "MultiHudWidget: Failed to construct widgets. GameState was invalid.");
		return;
	}

	TArray<UNamedSlot*> HudSlots = { PlayerSlot1, PlayerSlot2, PlayerSlot3, PlayerSlot4 };
	for (int i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		ADeckPlayerState* PlayerState = Cast<ADeckPlayerState>(GameState->PlayerArray[i]);
		if (!PlayerState)
		{
			continue;
		}

		UNamedSlot* PanelSlot = HudSlots[i];
		ConstructPlayerHud(PlayerState, PanelSlot);
	}
}

void UMultiHudWidget::ConstructPlayerHud(ADeckPlayerState* PlayerState, UNamedSlot* NamedSlot)
{
	UUserWidget* NewPlayerHud = CreateWidget(this, HudClass);
	if (!NewPlayerHud || !NewPlayerHud->Implements<UMultiplayerBindWidgetInterface>())
	{
		UE_LOGFMT(LogDeckUI, Error, "MultiHudWidget: Failed to construct player widget for PlayerState '{PlayerStateName}'.", PlayerState->GetName());
		return;
	}
	NamedSlot->AddChild(NewPlayerHud);

	IMultiplayerBindWidgetInterface::Execute_BindPlayer(NewPlayerHud, PlayerState);
}
