// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Tasks/StateTreeRunEnvQueryRepeatedTask.h"

#include "AI/DeckAIController.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Tasks/StateTreeRunEnvQueryTask.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeRunEnvQueryRepeatedTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.QueryTemplate)
	{
		return EStateTreeRunStatus::Failed;
	}

	StartQuery(Context, InstanceData);
	return InstanceData.RequestId != INDEX_NONE ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FStateTreeRunEnvQueryRepeatedTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	InstanceData.CurrentTimeInterval += DeltaTime;
	if (InstanceData.CurrentTimeInterval >= DeltaTime)
	{
		InstanceData.CurrentTimeInterval = 0.0f;
		// InstanceData.QueryResult.Reset();
		StartQuery(Context, InstanceData);
	}
	
	if (!InstanceData.QueryResult)
	{
		return EStateTreeRunStatus::Running;
	}
	
	if (!InstanceData.QueryResult->IsSuccessful())
	{
		return EStateTreeRunStatus::Running;
	}
	
	auto [VectorPtr, ActorPtr, ArrayOfVector, ArrayOfActor] = InstanceData.Result.GetMutablePtrTuple<FVector, AActor*, TArray<FVector>, TArray<AActor*>>(Context);
	if (VectorPtr)
	{
		*VectorPtr = InstanceData.QueryResult->GetItemAsLocation(0);
		/*if (InstanceData.TargetLocationProxy.IsValid())
		{
			InstanceData.TargetLocationProxy->SetActorLocation(*VectorPtr);
		}*/
		if (ADeckAIController* DeckAIController = Cast<ADeckAIController>(Context.GetOwner()))
		{
			if (DeckAIController->GetTargetLocationProxy())
			{
				DeckAIController->GetTargetLocationProxy()->SetActorLocation(*VectorPtr);
			}
		}
	}
	else if (ActorPtr)
	{
		*ActorPtr = InstanceData.QueryResult->GetItemAsActor(0);
	}
	else if (ArrayOfVector)
	{
		InstanceData.QueryResult->GetAllAsLocations(*ArrayOfVector);
	}
	else if (ArrayOfActor)
	{
		InstanceData.QueryResult->GetAllAsActors(*ArrayOfActor);
	}
	return EStateTreeRunStatus::Running;
}


void FStateTreeRunEnvQueryRepeatedTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	CancelQuery(Context, Context.GetInstanceData(*this));
}

void FStateTreeRunEnvQueryRepeatedTask::StartQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const
{
	AActor* Owner = InstanceData.QueryOwner;
	/*if (AAIController* Controller = Cast<AAIController>(InstanceData.QueryOwner))
	{
		Owner = Controller->GetPawn();
	}*/
	FEnvQueryRequest Request(InstanceData.QueryTemplate, Owner);

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

void FStateTreeRunEnvQueryRepeatedTask::CancelQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const
{
	if (InstanceData.RequestId != INDEX_NONE)
	{
		if (UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(Context.GetOwner()))
		{
			QueryManager->AbortQuery(InstanceData.RequestId);
		}
		InstanceData.RequestId = INDEX_NONE;
	}
	// InstanceData.QueryResult.Reset();
}

void FStateTreeRunEnvQueryRepeatedTask::ResetQuery(FStateTreeExecutionContext& Context,
	FInstanceDataType& InstanceData) const
{
	CancelQuery(Context, InstanceData);
	StartQuery(Context, InstanceData);
}

#if WITH_EDITOR
void FStateTreeRunEnvQueryRepeatedTask::PostEditInstanceDataChangeChainProperty(const FPropertyChangedChainEvent& PropertyChangedEvent, FStateTreeDataView InstanceDataView)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FStateTreeRunEnvQueryRepeatedInstanceData, QueryTemplate))
	{
		FInstanceDataType& InstanceData = InstanceDataView.GetMutable<FInstanceDataType>();
		if (InstanceData.QueryTemplate)
		{
			InstanceData.QueryTemplate->CollectQueryParams(*InstanceData.QueryTemplate, InstanceData.QueryConfig);
			for (FAIDynamicParam& DynamicParam : InstanceData.QueryConfig)
			{
				DynamicParam.bAllowBBKey = false;
			}
		}
		else
		{
			InstanceData.QueryConfig.Reset();
		}
	}
	else if (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(FAIDynamicParam, bAllowBBKey))
	{
		FInstanceDataType& InstanceData = InstanceDataView.GetMutable<FInstanceDataType>();
		const int32 ChangedIndex = PropertyChangedEvent.GetArrayIndex(GET_MEMBER_NAME_CHECKED(FStateTreeRunEnvQueryInstanceData, QueryConfig).ToString());
		if (InstanceData.QueryConfig.IsValidIndex(ChangedIndex))
		{
			if (!InstanceData.QueryConfig[ChangedIndex].bAllowBBKey)
			{
				InstanceData.QueryConfig[ChangedIndex].BBKey.InvalidateResolvedKey();
			}
		}
	}
}

FText FStateTreeRunEnvQueryRepeatedTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	FText QueryTemplateValue = BindingLookup.GetBindingSourceDisplayName(FStateTreePropertyPath(ID, GET_MEMBER_NAME_CHECKED(FInstanceDataType, QueryTemplate)), Formatting);
	if (QueryTemplateValue.IsEmpty())
	{
		QueryTemplateValue = FText::FromString(GetNameSafe(InstanceData->QueryTemplate));
	}

	if (Formatting == EStateTreeNodeFormatting::RichText)
	{
		return FText::Format(LOCTEXT("RunEQSRepeatedRich", "<b>Run EQS Query Repeated</> {0}"), QueryTemplateValue);	
	}
	return FText::Format(LOCTEXT("RunEQSRepeated", "Run EQS Query Repeated {0}"), QueryTemplateValue);
}
#endif // WITH_EDITOR

#undef LOCTEXT_NAMESPACE
