// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StateTreeConsiderationBase.h"
#include "UObject/Object.h"
#include "StateTreeHasGameplayTagConsideration.generated.h"


class UMovementComponent;
USTRUCT()
struct DECKGAME_API FStateTreeHasGameplayTagConsiderationInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = In)
	TObjectPtr<AActor> AbilitySystem;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	FGameplayTag GameplayTag;
};

USTRUCT(DisplayName = "Has Gameplay Tag")
struct DECKGAME_API FStateTreeHasGameplayTagConsideration : public FStateTreeConsiderationCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeHasGameplayTagConsiderationInstanceData;

	//~ Begin FStateTreeNodeBase Interface
	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting = EStateTreeNodeFormatting::Text) const override;
	//~ End FStateTreeNodeBase Interface
#endif

protected:
	//~ Begin FStateTreeConsiderationBase Interface
	float GetScore(FStateTreeExecutionContext& Context) const override;
	//~ End FStateTreeConsiderationBase Interface
};
