// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "StateTreePropertyFunctionBase.h"
#include "CoreMinimal.h"
#include "FStateTreeSetActorLocationPropertyFunction.generated.h"

class UAISense;
class AAIController;

USTRUCT()
struct DECKGAME_API FStateTreeSetActorLocationPropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<AActor> Input = nullptr;

	UPROPERTY(EditAnywhere, Category = Parameter)
	TSubclassOf<UAISense> Sense;
};

USTRUCT(DisplayName = "Get Closest Perceived")
struct FStateTreeSetActorLocationPropertyFunction : public FStateTreePropertyFunctionCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStateTreeSetActorLocationPropertyFunctionInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const;
#endif
};
