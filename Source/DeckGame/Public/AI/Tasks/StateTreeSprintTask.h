// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeSprintTask.generated.h"

USTRUCT()
struct DECKGAME_API FStateTreeSprintTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACharacter> Pawn;
};

USTRUCT(meta = (DisplayName = "Enable Sprint", Category = "AI|Action"))
struct DECKGAME_API FStateTreeSprintTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	bool bSprintEnable = true;

	using FInstanceDataType = FStateTreeSprintTaskInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

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
