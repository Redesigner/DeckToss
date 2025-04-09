// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreePropertyRef.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "StateTreeSetActorToQueryLocationTask.generated.h"

struct FEnvQueryResult;
struct FAIDynamicParam;
class UEnvQuery;

USTRUCT()
struct FStateTreeSetActorToQueryLocationInstanceData
{
	GENERATED_BODY()

	// The query will be run with this actor has the owner object.
	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<AActor> QueryOwner = nullptr;

	// The query template to run
	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<UEnvQuery> QueryTemplate;

	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<AActor> Actor = nullptr;

	// Query config associated with the query template.
	UPROPERTY(EditAnywhere, EditFixedSize, Category = Parameter)
	TArray<FAIDynamicParam> QueryConfig;

	/** To use dynamic pathfinding, set the location of an empty actor instead*/
	UPROPERTY(EditAnywhere, Category = Parameter)
	TWeakObjectPtr<AActor> TargetLocationProxy;

	UPROPERTY(EditAnywhere, Category = Parameter)
	FVector TargetLocation;

	TSharedPtr<FEnvQueryResult> QueryResult = nullptr;

	int32 RequestId = INDEX_NONE;
	
};

USTRUCT(meta = (DisplayName = "Set Actor To Query Location", Category = "Query"))
struct DECKGAME_API FStateTreeSetActorToQueryLocationTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeSetActorToQueryLocationInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	void StartQuery(FStateTreeExecutionContext& Context, FInstanceDataType& InstanceData) const;

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
