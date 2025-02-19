// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "CardItem.generated.h"

class USphereComponent;

UCLASS()
class DECKGAME_API ACardItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ACardItem();

	FGameplayTag GetCardAbility() const;

	void EnableMovement();

	void DisableMovement();

	void DisableReplication();

	bool IsFake() const;

	void SetLastHeldAbilitySystemComponent(UAbilitySystemComponent* ASC);

	void AttachFakeTo(ACardItem* Other);

	ACardItem* GetFakeOwner() const;

	void SetVelocity(FVector Velocity);

	void SetThrown(bool bNewValue);

	bool bHeld = false;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess, Categories = "CardTag"))
	FGameplayTag CardAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomReplication, meta = (AllowPrivateAccess = true, ClampMin = 0.0f))
	float MaxTeleportDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomReplication, meta = (AllowPrivateAccess = true, ClampMin = 0.0f))
	float InterpolationRate = 1.0f;


	void Tick(float DeltaSeconds) override;


	float CurrentError = 0.0f;

	TWeakObjectPtr<ACardItem> FakeOwner;

	bool bIsFake = false;

	bool bIsShadowingReal = false;

protected:
	// The ASC of whichever actor held this last. This can be used to infer that they caused any sort of damage or other side effects
	TWeakObjectPtr<UAbilitySystemComponent> LastHolder;

	bool bThrown = false;
};
