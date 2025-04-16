// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreePropertyRef.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "StateTreeRunEnvQueryRepeatedTask.generated.h"

struct FEnvQueryResult;
struct FAIDynamicParam;
class UEnvQuery;

USTRUCT()
struct FStateTreeRunEnvQueryRepeatedInstanceData
{
	GENERATED_BODY()

	// Result of the query. If an array is binded, it will output all the created values otherwise it will output the best one.
	UPROPERTY(EditAnywhere, Category = Out, meta = (RefType = "/Script/CoreUObject.Vector, /Script/Engine.Actor", CanRefToArray))
	FStateTreePropertyRef Result;

	// The query will be run with this actor has the owner object.
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> QueryOwner = nullptr;

	// The query template to run
	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<UEnvQuery> QueryTemplate;

	// Query config associated with the query template.
	UPROPERTY(EditAnywhere, EditFixedSize, Category = Parameter)
	TArray<FAIDynamicParam> QueryConfig;

	/** determines which item will be stored (All = only first matching) */
	UPROPERTY(EditAnywhere, Category = Parameter)
	TEnumAsByte<EEnvQueryRunMode::Type> RunMode = EEnvQueryRunMode::SingleResult;

	/** To use dynamic pathfinding, set the location of an empty actor instead*/
	UPROPERTY(EditAnywhere, Category = Parameter)
	TWeakObjectPtr<AActor> TargetLocationProxy;

	UPROPERTY(EditAnywhere, Category = Parameter)
	FVector TargetLocation;

	TSharedPtr<FEnvQueryResult> QueryResult = nullptr;

	int32 RequestId = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, Category = Parameter, Meta = (ClampMin = 0.0f))
	float Interval = 0.0f;

	float CurrentTimeInterval = 0.0f;
	
};

// This cannot inherit from FStateTreeRunEnvQueryTask because the editor methods are not exported
// Is this a bug?
USTRUCT(meta = (DisplayName = "Run Env Query Repeated", Category = "Query"))
struct DECKGAME_API FStateTreeRunEnvQueryRepeatedTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeRunEnvQueryRepeatedInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	void StartQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;
	void CancelQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;
	void ResetQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;

#if WITH_EDITOR
	void PostEditInstanceDataChangeChainProperty(const FPropertyChangedChainEvent& PropertyChangedEvent, FStateTreeDataView InstanceDataView) override;
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Find");
	}
	virtual FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Grey;
	}
#endif // WITH_EDITOR
};
