// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "ViewModel/HealthViewModel.h"

#include "Logging/StructuredLog.h"

#include "DeckUI.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/Attributes/BaseAttributeSet.h"

void UHealthViewModel::BindAttributeSet(UDeckAbilitySystemComponent* AbilitySystemComponent)
{
	if (!AbilitySystemComponent)
	{
		UE_LOGFMT(LogDeckUI, Error, "HealthViewModel: Failed to bind Attribute Set. The AbilitySystemComponent was invalid.");
		return;
	}

	const UBaseAttributeSet* AttributeSet = Cast<UBaseAttributeSet>(AbilitySystemComponent->GetAttributeSet(UBaseAttributeSet::StaticClass()));
	if (!AttributeSet)
	{
		UE_LOGFMT(LogDeckUI, Error, "HealthViewModel: Failed to bind Attribute Set. AbilitySystemComponent '{}' did not have a BaseAttributeSet.", AbilitySystemComponent->GetName());
		return;
	}

	// Bind our functions to the attributeset's appropriate delegates
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);

	// Broadcast changes for our initial value setting
	UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, AttributeSet->GetHealthAttribute().GetGameplayAttributeData(AttributeSet)->GetCurrentValue());
	UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, AttributeSet->GetMaxHealthAttribute().GetGameplayAttributeData(AttributeSet)->GetCurrentValue());
}

void UHealthViewModel::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_SET_PROPERTY_VALUE(CurrentHealth, Data.NewValue);
}

void UHealthViewModel::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_MVVM_SET_PROPERTY_VALUE(MaxHealth, Data.NewValue);
}
