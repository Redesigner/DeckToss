// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DeckMovementComponent.generated.h"

class UDeckAbilitySystemComponent;

UCLASS()
class DECKGAME_API UDeckMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Wandering, meta = (AllowPrivateAccess))
	float MaxWalkSpeedWandering = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Sprinting, meta = (AllowPrivateAccess, ClampMin = 1.0f))
	float SprintSpeedModifier = 1.25f;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Sprinting, meta = (AllowPrivateAccess))
	bool bIsSprinting = false;

	void SetASC(UDeckAbilitySystemComponent* ASC);

	void SetWandering(bool Wandering);


private:
	TWeakObjectPtr<UDeckAbilitySystemComponent> AbilitySystemComponent;

	bool bWandering = false;

	FVector RequestedMoveVelocity;
	
	UDeckMovementComponent();

	void SetUpdatedComponent(USceneComponent* Component) override;

	void PhysWalking(float DeltaTime, int32 Iterations) override;

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
};