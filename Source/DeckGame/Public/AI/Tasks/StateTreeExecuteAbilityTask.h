// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpecHandle.h"
#include "StateTreeTaskBase.h"
#include "StateTreeExecuteAbilityTask.generated.h"

enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct DECKGAME_API FStateTreeExecuteAbilityTaskInstanceData
{
	GENERATED_BODY()
	
	FGameplayAbilitySpecHandle ActivatedAbility;
	
	FDelegateHandle AbilityEndDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Context")
	TObjectPtr<AActor> Actor;
	
	bool bRunning = false;
};

USTRUCT(meta = (DisplayName = "Execute Ability Task"))
struct DECKGAME_API FStateTreeExecuteAbilityTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input", Meta=(Categories = "AbilityTag"))
	FGameplayTag AbilityTag;
	
	using FInstanceDataType = FStateTreeExecuteAbilityTaskInstanceData;

	FStateTreeExecuteAbilityTask() = default;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

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
