// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Combos/StateTreeComboSchema.h"

#include "AbilitySystemInterface.h"
#include "StateTreeExecutionContext.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/Combos/StateTreeComboComponent.h"

UStateTreeComboSchema::UStateTreeComboSchema()
{
	check(ContextDataDescs.Num() == 1 && ContextDataDescs[0].Struct == AActor::StaticClass());
	ContextDataDescs.Emplace(TEXT("Ability System Component"), UDeckAbilitySystemComponent::StaticClass(), FGuid(0xEDB3CD97, 0x95F94E0A, 0xBD15207B, 0x98645CDC));
}

bool UStateTreeComboSchema::SetContextRequirements(UBrainComponent& BrainComponent,
	FStateTreeExecutionContext& Context, bool bLogErrors)
{
	if (!Context.IsValid())
	{
		return false;
	}

	AActor* ContextActor = nullptr;
	if (const UStateTreeComboSchema* Schema = Cast<UStateTreeComboSchema>(Context.GetStateTree()->GetSchema()))
	{
		if (Context.GetOwner()->Implements<UAbilitySystemInterface>())
		{
			ContextActor = Cast<AActor>(Context.GetOwner());
		}
	}

	const FName ActorName(TEXT("Actor"));
	Context.SetContextDataByName(ActorName, FStateTreeDataView(ContextActor));

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(ContextActor))
	{
		if (UDeckAbilitySystemComponent* ASC = Cast<UDeckAbilitySystemComponent>(ASI->GetAbilitySystemComponent()))
		{
			const FName ASCName(TEXT("Ability System Component"));
			Context.SetContextDataByName(ASCName, FStateTreeDataView(ASC));
		}
	}	
	return Context.AreContextDataViewsValid();
}

bool UStateTreeComboSchema::CollectExternalData(const FStateTreeExecutionContext& Context, const UStateTree* StateTree,
	TArrayView<const FStateTreeExternalDataDesc> Descs, TArrayView<FStateTreeDataView> OutDataViews)
{
	return Super::CollectExternalData(Context, StateTree, Descs, OutDataViews);
}

void UStateTreeComboSchema::PostLoad()
{
	Super::PostLoad();
}
