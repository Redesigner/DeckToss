// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Properties/FStateTreeGetTargetLocationProxyPropertyFunction.h"

#include "DeckGame.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeNodeDescriptionHelpers.h"
#include "AI/DeckAIController.h"
#include "Logging/StructuredLog.h"

#define LOCTEXT_NAMESPACE "StateTree"

void FStateTreeGetTargetLocationProxyPropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.Input)
	{
		UE_LOGFMT(LogDeckGame, Warning, "FStateTree GetTargetLocationProxy: The input actor was null.");
		return;
	}
	
	InstanceData.Output = InstanceData.Input->GetTargetLocationProxy();

#if	WITH_EDITOR
	if (InstanceData.Output == nullptr)
	{
		UE_LOGFMT(LogDeckGame, Warning, "FStateTree GetTargetLocationProxy: The Target Location Proxy was null.");
	}
#endif
}

#if WITH_EDITOR
FText FStateTreeGetTargetLocationProxyPropertyFunction::GetDescription(const FGuid& ID,
	FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting) const
{
	return UE::StateTree::DescHelpers::GetDescriptionForSingleParameterFunc<FInstanceDataType>(LOCTEXT("StateTreeGetTargetLocationProxy", "GetTargetLocationProxy"), ID, InstanceDataView, BindingLookup, Formatting);
}
#endif

#undef LOCTEXT_NAMESPACE