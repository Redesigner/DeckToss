// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Properties/FStateTreeGetClosestPerceivedPropertyFunction.h"

#include "DeckGame.h"
#include "StateTreeExecutionContext.h"
#include "StateTreeNodeDescriptionHelpers.h"
#include "AI/DeckAIController.h"
#include "Logging/StructuredLog.h"
#include "Perception/AIPerceptionComponent.h"

#define LOCTEXT_NAMESPACE "StateTree"

void FStateTreeGetClosestPerceivedPropertyFunction::Execute(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.Input)
	{
		UE_LOGFMT(LogDeckGame, Warning, "FStateTree GetLastPerceived: The input controller was null.");
		return;
	}

	if (!InstanceData.InputActor)
	{
		UE_LOGFMT(LogDeckGame, Warning, "FStateTree GetLastPerceived: The input actor was null.");
		return;
	}
	
	if (UAIPerceptionComponent* PerceptionComponent = InstanceData.Input->GetPerceptionComponent())
	{
		TArray<AActor*> PerceivedActors;
		PerceptionComponent->GetCurrentlyPerceivedActors(InstanceData.Sense, PerceivedActors);

		if (PerceivedActors.Num() == 1)
		{
			InstanceData.OutputActor = PerceivedActors[0];
			if (InstanceData.OutputActor)
			{
				InstanceData.OutputLocation = InstanceData.OutputActor->GetActorLocation();
			}
			return;
		}
		
		float MaxDistanceSquared = -1.0f;
		const FVector ActorLocation = InstanceData.InputActor->GetActorLocation();
		for (AActor* PerceivedActor : PerceivedActors)
		{
			if (!PerceivedActor)
			{
				continue;
			}

			const FVector PerceivedLocation = PerceivedActor->GetActorLocation();
			const float CurrentDistance = FVector::DistSquared(PerceivedLocation, ActorLocation);
			if (CurrentDistance < MaxDistanceSquared || CurrentDistance < 0.0f)
			{
				InstanceData.OutputActor = PerceivedActor;
				InstanceData.OutputLocation = PerceivedLocation;
				MaxDistanceSquared = CurrentDistance;
			}
		}
	}
}

#if WITH_EDITOR
FText FStateTreeGetClosestPerceivedPropertyFunction::GetDescription(const FGuid& ID,
	FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup,
	EStateTreeNodeFormatting Formatting) const
{
	return UE::StateTree::DescHelpers::GetDescriptionForSingleParameterFunc<FInstanceDataType>(LOCTEXT("StateTreeGetClosestPerceived", "GetClosestPerceived"), ID, InstanceDataView, BindingLookup, Formatting);
}
#endif

#undef LOCTEXT_NAMESPACE