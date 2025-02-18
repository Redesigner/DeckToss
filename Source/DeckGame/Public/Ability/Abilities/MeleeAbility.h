// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "MeleeAbility.generated.h"

class UMeleeComponent;
class UGameplayEffect;

UCLASS()
class DECKGAME_API UMeleeAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> MeleeAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = true))
	float DamageAmount = 0.0f;


	TArray<TWeakObjectPtr<AActor>> DamagedActors;

	// A reference to the component, so it's easier to access from delegates
	TWeakObjectPtr<UMeleeComponent> MeleeComponent;
	FDelegateHandle ActorHitDelegateHandle;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)  override;
	
public:
	UFUNCTION()
	void ForceEndAbility();

	void OnEnemyHit(AActor* Enemy);
};
