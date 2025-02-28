// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "HealthViewModel.generated.h"

struct FOnAttributeChangeData;
class UDeckAbilitySystemComponent;

UCLASS()
class DECKUI_API UHealthViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	

	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = true))
	float CurrentHealth;

	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = true))
	float MaxHealth;

public:
	UFUNCTION(BlueprintCallable)
	void BindAttributeSet(UDeckAbilitySystemComponent* AbilitySystemComponent);

	void HealthChanged(const FOnAttributeChangeData& Data);

	void MaxHealthChanged(const FOnAttributeChangeData& Data);
};
