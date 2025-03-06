// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "StateTreeGetClosestPerceivedActor.h"

#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "StateTreeExecutionContext.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateGetClosestPerceivedActorTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.Actor)
	{
		return EStateTreeRunStatus::Failed;
	}

	if (!InstanceData.Controller)
	{
		return EStateTreeRunStatus::Failed;
	}

	UAIPerceptionComponent* AIPerception = InstanceData.Controller->PerceptionComponent;
	if (!InstanceData.Controller->PerceptionComponent)
	{
		return EStateTreeRunStatus::Failed;
	}

	TArray<AActor*> PerceivedActors;
	InstanceData.Controller->PerceptionComponent->GetKnownPerceivedActors(nullptr, PerceivedActors);
	if (PerceivedActors.IsEmpty())
	{
		return EStateTreeRunStatus::Failed;
	}

	FVector PawnLocation = InstanceData.Actor->GetActorLocation();
	FVector GoalLocation = FVector::ZeroVector;
	AActor* GoalActor = nullptr;
	
	float ShortestDistanceSquared = 100000000000.0f;
	for (AActor* PerceivedActor : PerceivedActors)
	{
		const float PerceivedDistance = FVector::DistSquared(PawnLocation, PerceivedActor->GetActorLocation());
		if (PerceivedDistance < ShortestDistanceSquared)
		{
			ShortestDistanceSquared = PerceivedDistance;
			GoalLocation = PerceivedActor->GetActorLocation();
			GoalActor = PerceivedActor;
		}
	}
	
	InstanceData.SensedLocation = GoalLocation;
	InstanceData.SensedActor = GoalActor;
	DrawDebugSphere(Context.GetWorld(), GoalLocation, 25.0f, 16, FColor::Red, false, 0.5f);
	return EStateTreeRunStatus::Running;
}

void FStateGetClosestPerceivedActorTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	Super::ExitState(Context, Transition);
}

FText FStateGetClosestPerceivedActorTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	return (Formatting == EStateTreeNodeFormatting::RichText)
		? LOCTEXT("GetPerceivedActorRich", "<b>Get Perceived Actor</>")
		: LOCTEXT("GetPerceivedActor", "GetPerceivedActor");
}
#undef LOCTEXT_NAMESPACE