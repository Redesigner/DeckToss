// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Properties/FStateTreeSetActorLocationPropertyFunction.h"

#include "DeckGame.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeNodeDescriptionHelpers.h"
#include "AI/DeckAIController.h"
#include "Logging/StructuredLog.h"

#define LOCTEXT_NAMESPACE "StateTree"

void FStateTreeSetActorLocationPropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
}

#if WITH_EDITOR
FText FStateTreeSetActorLocationPropertyFunction::GetDescription(const FGuid& ID,
	FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting) const
{
	return UE::StateTree::DescHelpers::GetDescriptionForSingleParameterFunc<FInstanceDataType>(LOCTEXT("StateTreeGetTargetLocationProxy", "GetTargetLocationProxy"), ID, InstanceDataView, BindingLookup, Formatting);
}
#endif

#undef LOCTEXT_NAMESPACE