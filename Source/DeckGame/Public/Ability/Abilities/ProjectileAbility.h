// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Ability/DeckGameplayAbility.h"
#include "Ability/Effects/GameplayEffectContainer.h"
#include "ProjectileAbility.generated.h"

UCLASS()
class DECKGAME_API UProjectileAbility : public UDeckGameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess))
	FGameplayEffectContainer EffectContainer;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Projectile, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Projectile, Meta = (AllowPrivateAccess))
	FVector ProjectileOffset;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Projectile, Meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float ProjectileLifetime;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Animation, Meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AnimationToPlay;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	void SpawnProjectile(FVector Location, FRotator Rotation);
};
