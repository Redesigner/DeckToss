// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "DeckGame.h"
#include "AI/Tasks/StateTreeSetActorToQueryLocationTask.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Logging/StructuredLog.h"
#include "Tasks/StateTreeRunEnvQueryTask.h"

#define LOCTEXT_NAMESPACE "StateTree"

EStateTreeRunStatus FStateTreeSetActorToQueryLocationTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.QueryTemplate)
	{
		return EStateTreeRunStatus::Failed;
	}

	StartQuery(Context, InstanceData);
	return InstanceData.RequestId != INDEX_NONE ? EStateTreeRunStatus::Running : EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FStateTreeSetActorToQueryLocationTask::Tick(FStateTreeExecutionContext& Context,
	const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	if (!InstanceData.QueryResult)
	{
		return EStateTreeRunStatus::Running;
	}
	
	if (!InstanceData.QueryResult->IsSuccessful())
	{
		return EStateTreeRunStatus::Failed;
	}

	FVector ResultLocation = InstanceData.QueryResult->GetItemAsLocation(0);
	if (!InstanceData.Actor)
	{
		UE_LOGFMT(LogDeckGame, Warning, "StateTree Set Actor to Query Location: task failed. The actor was null. Make sure to bind an actor in the StateTree.");
		return EStateTreeRunStatus::Failed;
	}

	InstanceData.Actor->SetActorLocation(ResultLocation);
	return EStateTreeRunStatus::Succeeded;
}

void FStateTreeSetActorToQueryLocationTask::ExitState(FStateTreeExecutionContext& Context,
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
	InstanceData.QueryResult.Reset();}

void FStateTreeSetActorToQueryLocationTask::StartQuery(FStateTreeExecutionContext& Context,
                                                       FInstanceDataType& InstanceData) const
{
	AActor* Owner = InstanceData.QueryOwner;
	FEnvQueryRequest Request(InstanceData.QueryTemplate, Owner);

	for (FAIDynamicParam& DynamicParam : InstanceData.QueryConfig)
	{
		Request.SetDynamicParam(DynamicParam, nullptr);
	}

	InstanceData.RequestId = Request.Execute(EEnvQueryRunMode::SingleResult,
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
void FStateTreeSetActorToQueryLocationTask::PostEditInstanceDataChangeChainProperty(
	const FPropertyChangedChainEvent& PropertyChangedEvent, FStateTreeDataView InstanceDataView)
{
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(FStateTreeSetActorToQueryLocationInstanceData, QueryTemplate))
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
	}}

FText FStateTreeSetActorToQueryLocationTask::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
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
		return FText::Format(LOCTEXT("SetActorToQueryLocation", "<b>Set Actor to Query Location</> {0}"), QueryTemplateValue);	
	}
	return FText::Format(LOCTEXT("SetActorToQueryLocation", "Set Actor To Query Location {0}"), QueryTemplateValue);}
#endif

#undef LOCTEXT_NAMESPACE