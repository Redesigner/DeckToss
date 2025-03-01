// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayEffect.h"
#include "AbilitySystemInterface.h"

#include "DeckEnemy.generated.h"

struct FActiveGameplayEffectHandle;
class UBaseAttributeSet;
class UDeckAbilitySet;
class UDeckAbilitySystemComponent;

UCLASS()
class DECKGAME_API ADeckEnemy : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UDeckAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities, Meta = (AllowPrivateAccess))
	TArray<TObjectPtr<UDeckAbilitySet>> AbilitySets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities, Meta = (AllowPrivateAccess))
	TObjectPtr<UBaseAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	bool bAlive = true;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, AActor*, Source, FHitResult, Hit);
	FOnTakeDamage OnTakeDamage;

private:
	ADeckEnemy();

	void BeginPlay() override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void GameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	void OnDeath(const FGameplayEffectSpec EffectSpec);
};
