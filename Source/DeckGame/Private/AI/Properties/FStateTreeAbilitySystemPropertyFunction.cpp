// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Properties/FStateTreeAbilitySystemPropertyFunction.h"

#include "AbilitySystemInterface.h"
#include "DeckGame.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeNodeDescriptionHelpers.h"
#include "Logging/StructuredLog.h"

#define LOCTEXT_NAMESPACE "StateTree"

void FStateTreeAbilitySystemPropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	if (!InstanceData.Input)
	{
		UE_LOGFMT(LogDeckGame, Warning, "FStateTree AbilitySystem: The input AbilitySystemInterface was null.");
		return;
	}

	if (IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(InstanceData.Input))
	{
		InstanceData.Output = AbilitySystem->GetAbilitySystemComponent();
	}
}

#if WITH_EDITOR
FText FStateTreeAbilitySystemPropertyFunction::GetDescription(const FGuid& ID,
	FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting) const
{
	return FText::FromString("Test");
	// return UE::StateTree::DescHelpers::GetDescriptionForSingleParameterFunc<FInstanceDataType>(LOCTEXT("StateTreeAbilitySystem", "GetAbilitySystem"), ID, InstanceDataView, BindingLookup, Formatting);
}
#endif

#undef LOCTEXT_NAMESPACE