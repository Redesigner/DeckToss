// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffect.h"
#include "Components/ActorComponent.h"
#include "EffectApplierComponent.generated.h"

/// This class is from my WitchForest project!

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DECKGAME_API UEffectApplierComponent : public UActorComponent
{
	GENERATED_BODY()

	/// Effects that are applied to actors that enter this area. These do not have an owner component
	/// If you want the effects to be applied from an owner, have the owner create spec(s) and se them with
	/// SetEffectHandles instead.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Effects, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	TArray<FGameplayEffectSpecHandle> EffectHandles;

	// The actor that set our effects. Filtered out by collision events
	TWeakObjectPtr<AActor> OwningActor;

public:
	void SetEffectHandles(AActor* OwningActorIn, TArray<FGameplayEffectSpecHandle> InHandles);

	TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectsToTarget(AActor* Target, const FHitResult& HitResult = FHitResult());

	AActor* GetOwningActor() const { return OwningActor.Get(); }
};
