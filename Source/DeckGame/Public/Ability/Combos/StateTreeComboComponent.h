// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "StateTreeComboComponent.generated.h"


UCLASS(ClassGroup= (Combo), meta=(BlueprintSpawnableComponent))
class DECKGAME_API UStateTreeComboComponent : public UStateTreeComponent
{
	GENERATED_BODY()
	
public:
	virtual TSubclassOf<UStateTreeSchema> GetSchema() const override;

	bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;
};
