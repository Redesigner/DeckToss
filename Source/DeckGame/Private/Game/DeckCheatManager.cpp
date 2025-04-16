// Copyright (c) 2024 Stephen Melnick


#include "Game/DeckCheatManager.h"

#include "Ability/DeckGameplayTags.h"
#include "Character/DeckPlayerController.h"
#include "Character/DeckPlayerState.h"

#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/Attributes/BaseAttributeSet.h"


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

	ApplyTag(ASC, DeckGameplayTags::Cheat_Immortal, bImmortal);
}

void UDeckCheatManager::SetInvulnerable(bool bInvuln)
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

	ApplyTag(ASC, DeckGameplayTags::GameplayEffect_Invuln, bInvuln);
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

void UDeckCheatManager::ApplyTag(UAbilitySystemComponent* ASC, FGameplayTag Tag, bool bApply)
{
	if (bApply)
	{
		ASC->AddLooseGameplayTag(Tag);
	}
	else
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}
