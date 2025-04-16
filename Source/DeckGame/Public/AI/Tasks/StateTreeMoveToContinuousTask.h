// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "AISystem.h"
#include "AITypes.h"
#include "StateTreeTaskBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "StateTreeMoveToContinuousTask.generated.h"

class UAITask_MoveTo;
class AAIController;

USTRUCT()
struct  DECKGAME_API FStateTreeMoveToContinuousTaskInstanceData
{
	GENERATED_BODY()

	// MOVE TO TASK PARAMETERS
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AAIController> AIController = nullptr;

	UPROPERTY(EditAnywhere, Category = Parameter)
	FVector Destination = FVector::Zero();

	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<AActor> TargetActor;

	/** fixed distance added to threshold between AI and goal location in destination reach test */
	UPROPERTY(EditAnywhere, Category = Parameter, meta=(ClampMin = "0.0", UIMin="0.0"))
	float AcceptableRadius = GET_AI_CONFIG_VAR(AcceptanceRadius);

	/** if the task is expected to react to changes to location in input
	 *	this property can be used to tweak sensitivity of the mechanism. Value is 
	 *	recommended to be less than AcceptableRadius */
	UPROPERTY(EditAnywhere, Category = Parameter, meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition="bTrackMovingGoal"))
	float DestinationMoveTolerance = 0.f;

	/** "None" will result in default filter being used */
	UPROPERTY(EditAnywhere, Category = Parameter)
	TSubclassOf<UNavigationQueryFilter> FilterClass;

	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bAllowStrafe = GET_AI_CONFIG_VAR(bAllowStrafing);

	/** if set, use incomplete path when goal can't be reached */
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bAllowPartialPath = GET_AI_CONFIG_VAR(bAcceptPartialPaths);

	/** if set, path to goal actor will update itself when actor moves */
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bTrackMovingGoal = true;

	/** if set, the goal location will need to be navigable */
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bRequireNavigableEndLocation = true;

	/** if set, goal location will be projected on navigation data (navmesh) before using */
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bProjectGoalLocation = true;

	/** if set, radius of AI's capsule will be added to threshold between AI and goal location in destination reach test  */
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bReachTestIncludesAgentRadius = GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap);
	
	/** if set, radius of goal's capsule will be added to threshold between AI and goal location in destination reach test  */
	UPROPERTY(EditAnywhere, Category = Parameter)
	bool bReachTestIncludesGoalRadius = GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap);

	UPROPERTY(Transient)
	TObjectPtr<UAITask_MoveTo> MoveToTask = nullptr;

	UPROPERTY(Transient)
	TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner = nullptr;
	// END MOVE TO TASK PARAMETERS

	// ========== RUN ENV QUERY PARAMETERS ============
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

	TSharedPtr<FEnvQueryResult> QueryResult = nullptr;

	int32 RequestId = INDEX_NONE;
	// ========= END RUN ENV QUERY PARAMETERS =============
	
	UPROPERTY(EditAnywhere, Category = Parameter, Meta = (ClampMin = 0.0f))
	float TimeInterval = 0.0f;

	float CurrentTimer = 0.0f;
};

/**
 * Move to a location from a continuously updating environment query
 */
USTRUCT(meta = (DisplayName = "Move To Continuous"))
struct DECKGAME_API FStateTreeMoveToContinuousTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeMoveToContinuousTaskInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }


	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	EStateTreeRunStatus PerformMoveTask(FStateTreeExecutionContext& Context, AAIController& Controller) const;
	UAITask_MoveTo* PrepareMoveToTask(FStateTreeExecutionContext& Context, AAIController& Controller, UAITask_MoveTo* ExistingTask, FAIMoveRequest& MoveRequest) const;
	void PrepareEnvQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Task");
	}
	FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Grey;
	}
#endif
	
};