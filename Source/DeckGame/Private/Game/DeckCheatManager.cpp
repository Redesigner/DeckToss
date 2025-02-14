// Copyright (c) 2024 Stephen Melnick


#include "Game/DeckCheatManager.h"

#include "DeckGame.h"
#include "Ability/DeckGameplayTags.h"
#include "Character/DeckPlayerController.h"
#include "Character/DeckPlayerState.h"
#include "Game/DeckGameMode.h"
#include "Game/DeckGameState.h"

#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/Attributes/BaseAttributeSet.h"

#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

void UDeckCheatManager::SetImmortal(bool bImmortal)
{
	ADeckPlayerController* DeckPlayerController = Cast<ADeckPlayerController>(GetOuterAPlayerController());
	if (!DeckPlayerController)
	{
		return;
	}

	ADeckPlayerState* DeckPlayerState = DeckPlayerController->GetPlayerState<ADeckPlayerState>();
	if (!DeckPlayerState)
	{
		return;
	}

	UDeckAbilitySystemComponent* ASC = DeckPlayerState->GetDeckAbilitySystemComponent();
	if (!ASC)
	{
		return;
	}

	if (bImmortal)
	{
		ASC->AddLooseGameplayTag(DeckGameplayTags::Cheat_Immortal);
	}
	else
	{
		ASC->RemoveLooseGameplayTag(DeckGameplayTags::Cheat_Immortal);
	}
}

void UDeckCheatManager::KillPlayer()
{
	ADeckPlayerController* DeckPlayerController = Cast<ADeckPlayerController>(GetOuterAPlayerController());
	if (!DeckPlayerController)
	{
		return;
	}

	ADeckPlayerState* DeckPlayerState = DeckPlayerController->GetPlayerState<ADeckPlayerState>();
	if (!DeckPlayerState)
	{
		return;
	}

	DeckPlayerState->GetAttributeSet()->KillOwner();
}