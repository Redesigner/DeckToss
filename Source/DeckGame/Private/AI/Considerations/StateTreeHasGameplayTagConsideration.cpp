// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/Considerations/StateTreeHasGameplayTagConsideration.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "StateTreeExecutionContext.h"

#define LOCTEXT_NAMESPACE "StateTree"

#if WITH_EDITOR
FText FStateTreeHasGameplayTagConsideration::GetDescription(const FGuid& ID, FStateTreeDataView InstanceDataView,
	const IStateTreeBindingLookup& BindingLookup, EStateTreeNodeFormatting Formatting) const
{
	const FInstanceDataType* InstanceData = InstanceDataView.GetPtr<FInstanceDataType>();
	check(InstanceData);

	return FText::FromString("Test");
	// return FText::Format(LOCTEXT("HasGameplayTag", "{AbilitySystemComponent} has GameplayTag {GameplayTag}"), 
	// 	FText::FromString(GetNameSafe(InstanceData->AbilitySystemComponent)), FText::FromString(InstanceData->GameplayTag.ToString()));
}
#endif

float FStateTreeHasGameplayTagConsideration::GetScore(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.AbilitySystem)
	{
		return 0.0f;
	}

	if (IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(InstanceData.AbilitySystem))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent())
		{
			float Value = AbilitySystemComponent->HasMatchingGameplayTag(InstanceData.GameplayTag) ? 1.0f : 0.0f;
			return Value;
		}
	}
	
	return 0.0f;
}

#undef LOCTEXT_NAMESPACE