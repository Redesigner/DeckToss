// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "GameObjects/Projectile.h"

#include "Ability/Effects/EffectApplierComponent.h"


AProjectile::AProjectile()
{
	EffectApplier = CreateDefaultSubobject<UEffectApplierComponent>(TEXT("Effect Applier"));

	PrimaryActorTick.bCanEverTick = true;
}

void AProjectile::IgnoreActor(AActor* Actor)
{
	IgnoredActors.Add(Actor);
}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IgnoredActors.Contains(OtherActor))
	{
		return;
	}

	const FHitResult FakeHit = FHitResult(OtherActor, nullptr, GetActorLocation(), -GetActorForwardVector());
	EffectApplier->ApplyGameplayEffectsToTarget(OtherActor, FakeHit);
}

void AProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * Speed * DeltaSeconds);
}
