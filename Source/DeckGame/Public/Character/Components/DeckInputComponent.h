// Copyright (c) 2024 Stephen Melnick, Epic Games

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"

#include "Input/DeckInputConfig.h"

#include "DeckInputComponent.generated.h"


UCLASS()
class DECKGAME_API UDeckInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	UDeckInputComponent(const FObjectInitializer& ObjectInitializer);

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UDeckInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UDeckInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};


template<class UserClass, typename FuncType>
void UDeckInputComponent::BindNativeAction(const UDeckInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* InputAction = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(InputAction, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UDeckInputComponent::BindAbilityActions(const UDeckInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FDeckInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}
