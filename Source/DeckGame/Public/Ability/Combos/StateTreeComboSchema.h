// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponentSchema.h"
#include "StateTreeComboSchema.generated.h"

struct FStateTreeExecutionContext;

class UDeckAbilitySystemComponent;
class UStateTree;

UCLASS()
class DECKGAME_API UStateTreeComboSchema : public UStateTreeComponentSchema
{
	GENERATED_BODY()
	
public:
	UStateTreeComboSchema();
	
	static bool SetContextRequirements(UBrainComponent& ComboComponent, FStateTreeExecutionContext& Context, bool bLogErrors = false);
	static bool CollectExternalData(const FStateTreeExecutionContext& Context, const UStateTree* StateTree, TArrayView<const FStateTreeExternalDataDesc> Descs, TArrayView<FStateTreeDataView> OutDataViews);
	void PostLoad() override;
};
