// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Tasks/StateTreeMoveToContinuousTask.h"

#include "AIController.h"
#include "AITypes.h"
#include "DeckGame.h"
#include "StateTreeExecutionContext.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Logging/StructuredLog.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "Tasks/AITask_MoveTo.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeMoveToContinuousTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.CurrentTimer = 0.0f;
	PrepareEnvQuery(Context, InstanceData);
	if (InstanceData.RequestId == INDEX_NONE)
	{
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.TaskOwner = TScriptInterface<IGameplayTaskOwnerInterface>(InstanceData.AIController->FindComponentByInterface(UGameplayTaskOwnerInterface::StaticClass()));
	if (!InstanceData.TaskOwner)
	{
		InstanceData.TaskOwner = InstanceData.AIController;
	}
	
	PerformMoveTask(Context, *InstanceData.AIController);
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStateTreeMoveToContinuousTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.CurrentTimer += DeltaTime;
	// Reset the env query, and run it again
	if (InstanceData.CurrentTimer >= InstanceData.TimeInterval)
	{
		if (InstanceData.RequestId != INDEX_NONE)
		{
			if (UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(Context.GetOwner()))
			{
				QueryManager->AbortQuery(InstanceData.RequestId);
			}
			InstanceData.RequestId = INDEX_NONE;
		}
		InstanceData.CurrentTimer = 0.0f;
		PrepareEnvQuery(Context, InstanceData);

		// Check the execution of the env query
		if (InstanceData.RequestId == INDEX_NONE)
		{
			return EStateTreeRunStatus::Failed;
		}
	}

	if (InstanceData.QueryResult)
	{
		// Make a copy so we can reset the instance data's copy
		TSharedPtr<FEnvQueryResult> QueryResultCopy = InstanceData.QueryResult;
		InstanceData.QueryResult.Reset();
		
		if (QueryResultCopy->IsSuccessful())
		{
			if (AActor* Actor = QueryResultCopy->GetItemAsActor(0))
			{
				if (InstanceData.TargetActor != Actor)
				{
					//InstanceData.MoveToTask->GetMoveRequestRef().SetGoalActor(Actor);
					InstanceData.TargetActor = Actor;
					return PerformMoveTask(Context, *InstanceData.AIController);
				}
			}
			else
			{
				FVector ResultLocation = QueryResultCopy->GetItemAsLocation(0);
				DrawDebugSphere(InstanceData.AIController->GetWorld(), ResultLocation, 50.0f, 16, FColor::Red, false, 0.02f);
				if (!ResultLocation.Equals(InstanceData.Destination))
				{
					//InstanceData.MoveToTask->GetMoveRequestRef().UpdateGoalLocation(ResultLocation);
					UE_LOGFMT(LogDeckGame, Display, "Updated query location after '{DeltaTime}' to '{Location}'", DeltaTime, ResultLocation.ToString());
					InstanceData.Destination = ResultLocation;
					return PerformMoveTask(Context, *InstanceData.AIController);
				}
			}
		}
		else
		{
			return EStateTreeRunStatus::Failed;
		}
	}
	return EStateTreeRunStatus::Running;
}

void FStateTreeMoveToContinuousTask::ExitState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (InstanceData.RequestId != INDEX_NONE)
	{
		if (UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(Context.GetOwner()))
		{
			QueryManager->AbortQuery(InstanceData.RequestId);
		}
		InstanceData.RequestId = INDEX_NONE;
	}
	InstanceData.QueryResult.Reset();

	if (InstanceData.MoveToTask && InstanceData.MoveToTask->GetState() != EGameplayTaskState::Finished)
	{
		UE_VLOG(Context.GetOwner(), LogStateTree, Log, TEXT("FStateTreeMoveToTaskContinuous aborting move to because state finished."));
		InstanceData.MoveToTask->ExternalCancel();
	}
}

EStateTreeRunStatus FStateTreeMoveToContinuousTask::PerformMoveTask(FStateTreeExecutionContext& Context,
                                                                    AAIController& Controller) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.TargetActor && InstanceData.MoveToTask)
	{
		if (InstanceData.MoveToTask->GetState() == EGameplayTaskState::Finished)
		{
			InstanceData.MoveToTask = nullptr;
		}
		else
		{
			FAIMoveRequest& MoveRequestHandle = InstanceData.MoveToTask->GetMoveRequestRef();
			MoveRequestHandle.UpdateGoalLocation(InstanceData.Destination);
			return EStateTreeRunStatus::Running;
		}
	}
	
	FAIMoveRequest MoveReq;
	MoveReq.SetNavigationFilter(InstanceData.FilterClass ? InstanceData.FilterClass : Controller.GetDefaultNavigationFilterClass())
		.SetAllowPartialPath(InstanceData.bAllowPartialPath)
		.SetAcceptanceRadius(InstanceData.AcceptableRadius)
		.SetCanStrafe(InstanceData.bAllowStrafe)
		.SetReachTestIncludesAgentRadius(InstanceData.bReachTestIncludesAgentRadius)
		.SetReachTestIncludesGoalRadius(InstanceData.bReachTestIncludesGoalRadius)
		.SetRequireNavigableEndLocation(InstanceData.bRequireNavigableEndLocation)
		.SetProjectGoalLocation(InstanceData.bProjectGoalLocation)
		.SetUsePathfinding(true);

	if (InstanceData.TargetActor)
	{
		if (InstanceData.bTrackMovingGoal)
		{
			MoveReq.SetGoalActor(InstanceData.TargetActor);
		}
		else
		{
			MoveReq.SetGoalLocation(InstanceData.TargetActor->GetActorLocation());
		}
	}
	else
	{
		MoveReq.SetGoalLocation(InstanceData.Destination);
	}

	if (MoveReq.IsValid())
	{	
		InstanceData.MoveToTask = PrepareMoveToTask(Context, Controller, InstanceData.MoveToTask, MoveReq);
		if (InstanceData.MoveToTask)
		{
			if (InstanceData.MoveToTask->IsActive())
			{
				InstanceData.MoveToTask->ConditionalPerformMove();
			}
			else
			{
				InstanceData.MoveToTask->ReadyForActivation();
			}

			if (InstanceData.MoveToTask->GetState() == EGameplayTaskState::Finished)
			{
				return InstanceData.MoveToTask->WasMoveSuccessful() ? EStateTreeRunStatus::Succeeded : EStateTreeRunStatus::Failed;
			}

			return EStateTreeRunStatus::Running;
		}
	}

	UE_VLOG(Context.GetOwner(), LogStateTree, Error, TEXT("FStateTreeMoveToContinuousTask failed because it doesn't have a destination."));
	return EStateTreeRunStatus::Failed;
}

UAITask_MoveTo* FStateTreeMoveToContinuousTask::PrepareMoveToTask(FStateTreeExecutionContext& Context,
	AAIController& Controller, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	UAITask_MoveTo* MoveTask = ExistingTask ? ExistingTask : UAITask::NewAITask<UAITask_MoveTo>(Controller, *InstanceData.TaskOwner);
	if (MoveTask)
	{
		MoveTask->SetUp(&Controller, MoveRequest);
	}

	return MoveTask;
}

void FStateTreeMoveToContinuousTask::PrepareEnvQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const
{
	FEnvQueryRequest Request(InstanceData.QueryTemplate, InstanceData.QueryOwner);

	for (FAIDynamicParam& DynamicParam : InstanceData.QueryConfig)
	{
		Request.SetDynamicParam(DynamicParam, nullptr);
	}

	InstanceData.RequestId = Request.Execute(InstanceData.RunMode,
		FQueryFinishedSignature::CreateLambda([InstanceDataRef = Context.GetInstanceDataStructRef(*this)](TSharedPtr<FEnvQueryResult> QueryResult) mutable
			{
				if (FInstanceDataType* InstanceData = InstanceDataRef.GetPtr())
				{
					InstanceData->QueryResult = QueryResult;
					InstanceData->RequestId = INDEX_NONE;
				}
			}));
}

#if WITH_EDITOR
FText FStateTreeMoveToContinuousTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	if (Formatting == EStateTreeNodeFormatting::RichText)
	{
		return LOCTEXT("MoveToContinuousRich", "<b>Move to Continuous</>");	
	}
	return LOCTEXT("MoveToContinuous", "");
}
#endif
#undef LOCTEXT_NAMESPACE