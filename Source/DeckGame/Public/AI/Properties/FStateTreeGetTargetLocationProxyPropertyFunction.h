// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "StateTreePropertyFunctionBase.h"
#include "CoreMinimal.h"
#include "FStateTreeGetTargetLocationProxyPropertyFunction.generated.h"

class ADeckAIController;

USTRUCT()
struct DECKGAME_API FStateTreeGetTargetLocationProxyPropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<ADeckAIController> Input = nullptr;

	UPROPERTY(EditAnywhere, Category = Output)
	TObjectPtr<AActor> Output = nullptr;
};

USTRUCT(DisplayName = "Get Target Location Proxy")
struct FStateTreeGetTargetLocationProxyPropertyFunction : public FStateTreePropertyFunctionCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStateTreeGetTargetLocationProxyPropertyFunctionInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const;
#endif
};