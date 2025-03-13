// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Combos/StateTreeComboComponent.h"

#include "Ability/Combos/StateTreeComboSchema.h"

#include "StateTreeExecutionContext.h"

TSubclassOf<UStateTreeSchema> UStateTreeComboComponent::GetSchema() const
{
	return UStateTreeComboSchema::StaticClass();
}

bool UStateTreeComboComponent::SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors)
{
	Context.SetCollectExternalDataCallback(FOnCollectStateTreeExternalData::CreateUObject(this, &UStateTreeComboComponent::CollectExternalData));
	return UStateTreeComboSchema::SetContextRequirements(*this, Context, bLogErrors);
}
