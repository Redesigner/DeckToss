// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UEffectApplierComponent;

UCLASS()
class DECKGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Projectile, Meta = (AllowPrivateAccess))
	float Speed;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UEffectApplierComponent> EffectApplier;

public:
	void IgnoreActor(AActor* Actor);

private:
	AProjectile();

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void Tick(float DeltaSeconds) override;

	TArray<TWeakObjectPtr<AActor>> IgnoredActors;	
};
