// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Tasks/StateTreeSetFocusTargetTask.h"

#include "AIController.h"
#include "StateTreeExecutionContext.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeSetFocusTargetTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.Controller)
	{
		return EStateTreeRunStatus::Failed;
	}
		
	// FocusTarget can be nullptr!
	InstanceData.Controller->SetFocus(InstanceData.FocusTarget.Get());
	return EStateTreeRunStatus::Succeeded;
}

#if WITH_EDITOR
FText FStateTreeSetFocusTargetTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	FText FocusTargetName = BindingLookup.GetBindingSourceDisplayName(FStateTreePropertyPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, FocusTarget)), Formatting);
	if (FocusTargetName.IsEmpty())
	{
		FocusTargetName = FText::FromString(GetNameSafe(InstanceData->FocusTarget));
	}
	
	if (Formatting == EStateTreeNodeFormatting::RichText)
	{
		return FText::Format(LOCTEXT("SetActorToQueryLocation", "<b>Set Focus Target</> {0}"), FocusTargetName);	
	}
	return FText::Format(LOCTEXT("SetActorToQueryLocation", "Set Focus Target {0}"), FocusTargetName);
}
#endif
#undef LOCTEXT_NAMESPACE