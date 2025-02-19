// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "GameObjects/CardItem.h"

#include "Components/SphereComponent.h"

ACardItem::ACardItem()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = true;
    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
}

FGameplayTag ACardItem::GetCardAbility() const
{
	return CardAbility;
}

void ACardItem::EnableMovement()
{
    if (!CollisionSphere)
    {
        return;
    }

    CollisionSphere->SetSimulatePhysics(true);
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACardItem::DisableMovement()
{
    if (!CollisionSphere)
    {
        return;
    }

    CollisionSphere->SetSimulatePhysics(false);
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACardItem::DisableReplication()
{
    bIsFake = true;
    bReplicates = false;
}

bool ACardItem::IsFake() const
{
    return bIsFake;
}

ACardItem* ACardItem::GetFakeOwner() const
{
    return FakeOwner.Get();
}

void ACardItem::Tick(float DeltaSeconds)
{
    if (!bIsShadowingReal)
    {
        return;
    }

    if (!FakeOwner.IsValid())
    {
        Destroy();
        return;
    }

    FVector Delta = FakeOwner->GetActorLocation() - GetActorLocation();
    float SquaredLength = Delta.SquaredLength();
    if (SquaredLength <= 1.0f)
    {
        FakeOwner->SetActorHiddenInGame(false);
        Destroy();
        return;
    }

    if (SquaredLength >= MaxTeleportDistance * MaxTeleportDistance)
    {
        CurrentError += SquaredLength * DeltaSeconds;
    }

    if (CurrentError >= MaxTeleportDistance * MaxTeleportDistance)
    {
        FakeOwner->SetActorHiddenInGame(false);
        Destroy();
        return;
    }

    AddActorWorldOffset(Delta * FMath::Clamp(DeltaSeconds * InterpolationRate, 0.0f, 1.0f));
}
