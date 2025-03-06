// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Tasks/StateTreeExecuteAbilityTask.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Logging/StructuredLog.h"
#include "StateTreeExecutionContext.h"

#include "DeckGame.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeExecuteAbilityTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	
	IAbilitySystemInterface* AbilitySystem = Cast<IAbilitySystemInterface>(InstanceData.Actor);
	if (!AbilitySystem)
	{
		return EStateTreeRunStatus::Failed;
	}

	UAbilitySystemComponent* ASC = AbilitySystem->GetAbilitySystemComponent();
	if (!ASC)
	{
		return EStateTreeRunStatus::Failed;
	}

	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	ASC->FindAllAbilitiesWithTags(AbilityHandles, AbilityTag.GetSingleTagContainer());
	if (AbilityHandles.IsEmpty())
	{
		return EStateTreeRunStatus::Failed;
	}

	TWeakObjectPtr<UAbilitySystemComponent> OwningASC = ASC;
	InstanceData.AbilityEndDelegate = ASC->OnAbilityEnded.AddLambda([this, Context, OwningASC](const FAbilityEndedData& AbilityEndedData)
	{
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
		if (AbilityEndedData.AbilitySpecHandle != InstanceData.ActivatedAbility || !OwningASC.IsValid())
		{
			return;
		}

		UE_LOGFMT(LogDeckGame, Display, "STT_ExecuteAbility: Ability '{AbilityName}' ended.", AbilityEndedData.AbilityThatEnded->GetName());
		OwningASC->OnAbilityEnded.Remove(InstanceData.AbilityEndDelegate);
		InstanceData.ActivatedAbility = FGameplayAbilitySpecHandle();
		InstanceData.bRunning = false;
	});
	
	InstanceData.ActivatedAbility = AbilityHandles[0];
	if (ASC->TryActivateAbility(AbilityHandles[0]))
	{
		InstanceData.bRunning = true;
		return EStateTreeRunStatus::Running;
	}

	ASC->OnAbilityEnded.Remove(InstanceData.AbilityEndDelegate);
	InstanceData.ActivatedAbility = FGameplayAbilitySpecHandle();
	return EStateTreeRunStatus::Failed;
}

void FStateTreeExecuteAbilityTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
}

EStateTreeRunStatus FStateTreeExecuteAbilityTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// This seems to be the best way of returning this info to the execution, since it can't be called directly from a delegate
	if (InstanceData.bRunning)
	{
		return EStateTreeRunStatus::Running;
	}
	else
	{
		return EStateTreeRunStatus::Succeeded;
	}
}

#if WITH_EDITOR
FText FStateTreeExecuteAbilityTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
		? LOCTEXT("ExecuteAbilityRich", "<b>Execute Ability</> \"{AbilityName}\"")
		: LOCTEXT("ExecuteAbility", "ExecuteAbility \"{AbilityName}\"");

	return FText::FormatNamed(Format,
		TEXT("AbilityName"), FText::FromString(AbilityTag.ToString()));
}
#endif
#undef LOCTEXT_NAMESPACE
