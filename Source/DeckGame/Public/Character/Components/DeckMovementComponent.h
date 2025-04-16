// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActiveGameplayEffectHandle.h"

#include "DeckMovementComponent.generated.h"

struct FActiveGameplayEffect;
struct FGameplayEffectSpec;
class UAbilitySystemComponent;
class UDeckAbilitySystemComponent;

UCLASS()
class DECKGAME_API UDeckMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Wandering, meta = (AllowPrivateAccess))
	float MaxWalkSpeedWandering = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, meta = (AllowPrivateAccess, ClampMin = 1.0f))
	float SprintSpeedModifier = 1.25f;

	/// Stop sprinting when the character is not actively moving
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, meta = (AllowPrivateAccess))
	bool bStopSprintWhenStill = true;
	
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Sprinting, meta = (AllowPrivateAccess))
	bool bIsSprinting = false;

	void SetASC(UDeckAbilitySystemComponent* ASC);

	void SetWandering(bool Wandering);


private:
	TWeakObjectPtr<UDeckAbilitySystemComponent> AbilitySystemComponent;

	void OnGameplayEffectAppliedToOwner(UAbilitySystemComponent* Source, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	void OnGameplayEffectRemovedFromOwner(const FActiveGameplayEffect& ActiveGameplayEffect);

	bool bWandering = false;

	FVector RequestedMoveVelocity;
	
	UDeckMovementComponent();

	void SetUpdatedComponent(USceneComponent* Component) override;

	void PhysWalking(float DeltaTime, int32 Iterations) override;

	void PhysFalling(float deltaTime, int32 Iterations) override;

	FRotator GetDeltaRotation(float DeltaTime) const override;
	void PhysicsRotation(float DeltaTime) override;

	float GetMaxSpeed() const override;

	float GetMaxWalkSpeed() const;

	//  BEGIN INTERFACE Navigation Movement component
	void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	void RequestPathMove(const FVector& MoveInput) override;
	bool CanStartPathFollowing() const override;
	bool CanStopPathFollowing() const override;
	void StopActiveMovement() override;
	// END INTERFACE

	bool bStunned = false;
	bool bFrozen = false;
};