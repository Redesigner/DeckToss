// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "StateTreePropertyFunctionBase.h"
#include "CoreMinimal.h"
#include "FStateTreeAbilitySystemPropertyFunction.generated.h"

class UAbilitySystemComponent;
class UAISense;
class AAIController;

USTRUCT()
struct DECKGAME_API FStateTreeAbilitySystemPropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<AActor> Input = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Output)
	TObjectPtr<UAbilitySystemComponent> Output = nullptr;
};

USTRUCT(DisplayName = "Get Ability System Component")
struct FStateTreeAbilitySystemPropertyFunction : public FStateTreePropertyFunctionCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStateTreeAbilitySystemPropertyFunctionInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const;
#endif
};
