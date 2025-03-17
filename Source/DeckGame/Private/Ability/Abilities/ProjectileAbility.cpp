// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Abilities/ProjectileAbility.h"

#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Ability/Effects/EffectApplierComponent.h"
#include "GameObjects/Projectile.h"

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ProjectileClass)
	{
		UE_LOGFMT(LogDeckGame, Display, "ProjectileAbility '{AbilityName}' failed to execute. No projectile class was set.", GetName());
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	SpawnProjectile(ActorInfo->AvatarActor->GetActorLocation() + ProjectileOffset, ActorInfo->AvatarActor->GetActorRotation());
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UProjectileAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (ProjectileClass)
	{
		return true;
	}
	return false;
}

void UProjectileAbility::SpawnProjectile(FVector Location, FRotator Rotation)
{
	FTransform ProjectileTransform;
	ProjectileTransform.SetLocation(Location);
	ProjectileTransform.SetRotation(Rotation.Quaternion());
	AActor* Projectile = GetWorld()->SpawnActorDeferred<AActor>(ProjectileClass.Get(), ProjectileTransform);

	if (UEffectApplierComponent* EffectApplier = Projectile->GetComponentByClass<UEffectApplierComponent>())
	{
		EffectApplier->SetEffectHandles(CurrentActorInfo->AvatarActor.Get(), EffectContainer.MakeEffectSpecs(this));
	}
	if (AProjectile* SpawnedProjectile = Cast<AProjectile>(Projectile))
	{
		SpawnedProjectile->IgnoreActor(CurrentActorInfo->AvatarActor.Get());
	}
	
	Projectile->FinishSpawning(ProjectileTransform);
	if (ProjectileLifetime > 0.0f)
	{
		Projectile->SetLifeSpan(ProjectileLifetime);
	}
}
