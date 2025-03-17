// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Effects/EffectApplierComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Logging/StructuredLog.h"

#include "DeckGame.h"

void UEffectApplierComponent::SetEffectHandles(AActor* OwningActorIn, TArray<FGameplayEffectSpecHandle> InHandles)
{
	OwningActor = OwningActorIn;
	EffectHandles = InHandles;
}

TArray<FActiveGameplayEffectHandle> UEffectApplierComponent::ApplyGameplayEffectsToTarget(AActor* Target,
	const FHitResult& HitResult)
{
	TArray<FActiveGameplayEffectHandle> Result;

	if (!GetOwner())
	{
		UE_LOGFMT(LogDeckGame, Warning, "EffectApplicationComponent '{ComponentName}' was unable to apply a GameplayEffect. No owner was Found.", GetName());

	}
	
	if (!GetOwner()->HasAuthority() || !Target)
	{
		return Result;
	}

	if (EffectHandles.Num() == 0 && DefaultEffects.Num() == 0)
	{
		UE_LOGFMT(LogDeckGame, Warning, "EffectApplicationComponent {ComponentName} was unable to apply a GameplayEffect to {OtherActorName}. No GameplayEffectHandles were set. Make sure to call SetEffectHandles after spawning the projectile.", GetName(), Target->GetName());
		return Result;
	}

	if (IAbilitySystemInterface* ActorAbility = Cast<IAbilitySystemInterface>(Target))
	{
		for (FGameplayEffectSpecHandle EffectHandle : EffectHandles)
		{
			FGameplayEffectContextHandle EffectContext = EffectHandle.Data->GetContext();
			// We have to reset the effectcontext's hit results, otherwise we may be
			// trying to apply effects with hit results that have fallen out of scope,
			// since internally they are stored as shared ptrs
			EffectContext.AddHitResult(HitResult, true);

			// Owning actor is the avatar, so it can disappear while this remains valid,
			// for instance: a projectile will still be valid if its owner has died
			if (OwningActor.IsValid())
			{
				EffectContext.AddOrigin(OwningActor->GetActorLocation());
			}

			FGameplayEffectSpec* Spec = EffectHandle.Data.Get();
			if (!Spec)
			{
				continue;
			}


			if (!ActorAbility->GetAbilitySystemComponent())
			{
				// We have to check each application that the ASC is valid.
				// If, for instance, an applied effect kills the actor, the
				// ASC could be deleted or otherwise invalidated
				UE_LOGFMT(LogDeckGame, Warning, "Area of Effect {ProjectileName} was unable to apply a GameplayEffect to {OtherActorName}. The target ASC was invalid.", GetName(), Target->GetName());
				return Result;
			}

			Result.Add(ActorAbility->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*Spec));
		}

		for (TSubclassOf<UGameplayEffect> GameplayEffectClass : DefaultEffects)
		{
			if (!ActorAbility->GetAbilitySystemComponent())
			{
				// We have to check each application that the ASC is valid.
				// If, for instance, an applied effect kills the actor, the
				// ASC could be deleted or otherwise invalidated
				UE_LOGFMT(LogDeckGame, Warning, "Area of Effect {ProjectileName} was unable to apply a GameplayEffect to {OtherActorName}. The target ASC was invalid.", GetName(), Target->GetName());
				return Result;
			}

			if (!GameplayEffectClass)
			{
				UE_LOGFMT(LogDeckGame, Warning, "Area of Effect {ProjectileName} was unable to apply a GameplayEffect to {OtherActorName}. The GameplayEffectClass was invalid.", GetName(), Target->GetName());
				continue;
			}

			FGameplayEffectContextHandle ContextHandle = ActorAbility->GetAbilitySystemComponent()->MakeEffectContext();
			Result.Add(ActorAbility->GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(Cast<UGameplayEffect>(GameplayEffectClass->GetDefaultObject()), 1.0f, ContextHandle));
		}
	}
	return Result;

}
