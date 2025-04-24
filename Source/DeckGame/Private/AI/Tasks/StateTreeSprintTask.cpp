// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Tasks/StateTreeSprintTask.h"

#include "StateTreeExecutionContext.h"

#include "DeckGame.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/DeckGameplayAbility.h"
#include "Character/Components/DeckMovementComponent.h"
#include "GameFramework/Character.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeSprintTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.Pawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	if (UDeckMovementComponent* MovementComponent = Cast<UDeckMovementComponent>(InstanceData.Pawn->GetCharacterMovement()))
	{
		MovementComponent->bIsSprinting = bSprintEnable;
		return bEndTaskOnComplete ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Running;
	}

	return EStateTreeRunStatus::Failed;
}

#if WITH_EDITOR
FText FStateTreeSprintTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FText Format = (Formatting == EStateTreeNodeFormatting::RichText)
		? LOCTEXT("SprintTaskRich", "<b>{Bool}</> Sprint")
		: LOCTEXT("SprintTask", "{Bool} Sprint");

	return FText::FormatNamed(Format,
		TEXT("Bool"), FText::FromString(bSprintEnable ? "Enable" : "Disable"));
}
#endif
#undef LOCTEXT_NAMESPACE