// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Combos/StateTreeComboComponent.h"

#include "Logging/StructuredLog.h"
#include "StateTreeExecutionContext.h"

#include "DeckGame.h"
#include "Ability/Combos/StateTreeComboSchema.h"

UStateTreeComboComponent::UStateTreeComboComponent()
{
}

TSubclassOf<UStateTreeSchema> UStateTreeComboComponent::GetSchema() const
{
	return UStateTreeComboSchema::StaticClass();
}

bool UStateTreeComboComponent::SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors)
{
	Context.SetCollectExternalDataCallback(FOnCollectStateTreeExternalData::CreateUObject(this, &UStateTreeComboComponent::CollectExternalData));
	return UStateTreeComboSchema::SetContextRequirements(*this, Context, bLogErrors);
}

void UStateTreeComboComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputHeldTags.Contains(InputTag))
	{
		return;
	}
	UE_LOGFMT(LogDeckGame, Display, "StateTree Combo received input '{Tag}'", InputTag.ToString());
	InputHeldTags.Add(InputTag);
	SendStateTreeEvent(InputTag, FConstStructView::Make(InputTag));
}

void UStateTreeComboComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	InputHeldTags.Remove(InputTag);
}
