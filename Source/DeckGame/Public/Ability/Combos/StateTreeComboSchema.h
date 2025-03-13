// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "StateTreeSchema.h"
#include "StateTreeComboSchema.generated.h"

struct FStateTreeExecutionContext;

class UDeckAbilitySystemComponent;
class UStateTree;

UCLASS()
class DECKGAME_API UStateTreeComboSchema : public UStateTreeSchema
{
	GENERATED_BODY()
	
public:
	UStateTreeComboSchema();
	
	static bool SetContextRequirements(UStateTreeComboComponent& ComboComponent, FStateTreeExecutionContext& Context, bool bLogErrors = false);
	static bool CollectExternalData(const FStateTreeExecutionContext& Context, const UStateTree* StateTree, TArrayView<const FStateTreeExternalDataDesc> Descs, TArrayView<FStateTreeDataView> OutDataViews);

protected:
	bool IsStructAllowed(const UScriptStruct* InScriptStruct) const override;
	bool IsClassAllowed(const UClass* InScriptStruct) const override;
	bool IsExternalItemAllowed(const UStruct& InStruct) const override;
	
	TConstArrayView<FStateTreeExternalDataDesc> GetContextDataDescs() const override;

	void PostLoad() override;

#if WITH_EDITOR
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	
	const FStateTreeExternalDataDesc& GetContextActorDataDesc() const { return ContextDataDescs[0]; }
	FStateTreeExternalDataDesc& GetContextActorDataDesc() { return ContextDataDescs[0]; }

	/** Actor class the StateTree is expected to run on. Allows to bind to specific Actor class' properties. */
	UPROPERTY(EditAnywhere, Category="Defaults", NoClear)
	TSubclassOf<AActor> ContextActorClass;
	
	UPROPERTY()
	TArray<FStateTreeExternalDataDesc> ContextDataDescs;
};
