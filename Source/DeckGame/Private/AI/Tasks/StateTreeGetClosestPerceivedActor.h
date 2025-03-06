// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "StateTreeTaskBase.h"

#include "StateTreeGetClosestPerceivedActor.generated.h"

class AAIController;

USTRUCT()
struct DECKGAME_API FStateGetClosestPerceivedActorTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = "Output")
	FVector SensedLocation;

	UPROPERTY(VisibleAnywhere, Category = "Output")
	TObjectPtr<AActor> SensedActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Context")
	TObjectPtr<AActor> Actor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Context")
	TObjectPtr<AAIController> Controller;
};

USTRUCT(meta = (DisplayName = "Get Closest Perceived Actor Task"))
struct DECKGAME_API FStateGetClosestPerceivedActorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateGetClosestPerceivedActorTaskInstanceData;

	FStateGetClosestPerceivedActorTask() = default;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	FName GetIconName() const override
	{
		return FName("StateTreeEditorStyle|Node.Text");
	}
	FColor GetIconColor() const override
	{
		return UE::StateTree::Colors::Grey;
	}
#endif
};