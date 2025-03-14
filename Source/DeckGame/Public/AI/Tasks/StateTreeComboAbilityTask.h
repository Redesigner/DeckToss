// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "StateTreeTaskBase.h"
#include "StateTreeComboAbilityTask.generated.h"

class UDeckAbilitySystemComponent;
class UDeckGameplayAbility;
enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct DECKGAME_API FStateTreeComboAbilityTaskInstanceData
{
	GENERATED_BODY()
	
	FGameplayAbilitySpecHandle ActivatedAbility;
	
	FDelegateHandle AbilityEndDelegate;
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TWeakObjectPtr<UDeckAbilitySystemComponent> AbilitySystemComponent;
	
	bool bRunning = false;

	float TimerCount = 0.0f;
};

USTRUCT(meta = (DisplayName = "Combo Ability Task"))
struct DECKGAME_API FStateTreeComboAbilityTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	TSubclassOf<UDeckGameplayAbility> Ability;
	
	UPROPERTY(EditAnywhere, Category = "Input", Meta = (ClampMin = 0.0f))
	float DelayTime = 0.0f;

	using FInstanceDataType = FStateTreeComboAbilityTaskInstanceData;

	FStateTreeComboAbilityTask() = default;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

	EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

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
