// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Combos/StateTreeComboSchema.h"

UStateTreeComboSchema::UStateTreeComboSchema()
{
}

bool UStateTreeComboSchema::SetContextRequirements(UStateTreeComboComponent& ComboComponent,
	FStateTreeExecutionContext& Context, bool bLogErrors)
{
}

bool UStateTreeComboSchema::CollectExternalData(const FStateTreeExecutionContext& Context, const UStateTree* StateTree,
	TArrayView<const FStateTreeExternalDataDesc> Descs, TArrayView<FStateTreeDataView> OutDataViews)
{
}
