// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Tasks/StateTreeComboAbilityTask.h"

#include "Logging/StructuredLog.h"
#include "StateTreeExecutionContext.h"

#include "DeckGame.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/DeckGameplayAbility.h"

#define LOCTEXT_NAMESPACE "StateTree"

#if WITH_EDITOR
EStateTreeRunStatus FStateTreeComboAbilityTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.AbilitySystemComponent.IsValid())
	{
		UE_LOGFMT(LogDeckGame, Warning, "State Tree Combo: failed to execute task, the ASC was not valid.");
		return EStateTreeRunStatus::Failed;
	}

	if (!Ability)
	{
		return EStateTreeRunStatus::Failed;
	}

	FGameplayAbilitySpecHandle SpecToActivate;
	for (FGameplayAbilitySpec& Spec :InstanceData.AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.Ability.Get() == Ability.GetDefaultObject())
		{
			SpecToActivate = Spec.Handle;
			break;
		}
	}

	if (!SpecToActivate.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	InstanceData.AbilityEndDelegate = InstanceData.AbilitySystemComponent->OnAbilityEnded.AddLambda([this, Context](const FAbilityEndedData& AbilityEndedData)
	{
		FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
		if (AbilityEndedData.AbilitySpecHandle != InstanceData.ActivatedAbility || !InstanceData.AbilitySystemComponent.IsValid())
		{
			return;
		}

		UE_LOGFMT(LogDeckGame, Display, "STT_ComboAbility: Ability '{AbilityName}' ended.", AbilityEndedData.AbilityThatEnded->GetName());
		InstanceData.AbilitySystemComponent->OnAbilityEnded.Remove(InstanceData.AbilityEndDelegate);
		InstanceData.ActivatedAbility = FGameplayAbilitySpecHandle();
		InstanceData.bRunning = false;
	});

	InstanceData.ActivatedAbility = SpecToActivate;
	
	if (InstanceData.AbilitySystemComponent->TryActivateAbility(SpecToActivate))
	{
		InstanceData.bRunning = true;
		return EStateTreeRunStatus::Running;
	}

	UE_LOGFMT(LogDeckGame, Display, "STT_ComboAbility: Ability '{AbilityName}' failed to activate.", Ability->GetName());
	return EStateTreeRunStatus::Failed;
}

void FStateTreeComboAbilityTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FStateTreeTaskCommonBase::ExitState(Context, Transition);
}

EStateTreeRunStatus FStateTreeComboAbilityTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (InstanceData.bRunning)
	{
		return EStateTreeRunStatus::Running;
	}
	InstanceData.TimerCount += DeltaTime;

	if (InstanceData.TimerCount >= DelayTime)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
}

FText FStateTreeComboAbilityTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
                                                 const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
		? LOCTEXT("ComboAbilityRich", "<b>Combo Ability</> \"{AbilityName}\"")
		: LOCTEXT("ComboAbility", "ComboAbility \"{AbilityName}\"");

	return FText::FormatNamed(Format,
		TEXT("AbilityName"), FText::FromString(Ability ? Ability->GetName() : "None"));
}
#endif
#undef LOCTEXT_NAMESPACE