// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "StateTreePropertyFunctionBase.h"
#include "CoreMinimal.h"
#include "FStateTreeGetClosestPerceivedPropertyFunction.generated.h"

class UAISense;
class AAIController;

USTRUCT()
struct DECKGAME_API FStateTreeGetClosestPerceivedPropertyFunctionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<AAIController> Input = nullptr;
	
	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<AActor> InputActor = nullptr;

	UPROPERTY(EditAnywhere, Category = Parameter)
	TSubclassOf<UAISense> Sense;
	
	UPROPERTY(EditAnywhere, Category = Output)
	TObjectPtr<AActor> OutputActor = nullptr;

	UPROPERTY(EditAnywhere, Category = Output)
	FVector OutputLocation;
};

USTRUCT(DisplayName = "Get Closest Perceived")
struct FStateTreeGetClosestPerceivedPropertyFunction : public FStateTreePropertyFunctionCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStateTreeGetClosestPerceivedPropertyFunctionInstanceData;

	const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	void Execute(FStateTreeExecutionContext& Context) const override;

#if WITH_EDITOR
	FText GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView, const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const;
#endif
};
