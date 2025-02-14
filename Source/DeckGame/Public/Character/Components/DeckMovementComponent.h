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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Wandering, meta = (AllowPrivateAccess = true))
	float MaxWalkSpeedWandering = 100.0f;


	TWeakObjectPtr<UDeckAbilitySystemComponent> AbilitySystemComponent;

	bool bWandering = false;


	UDeckMovementComponent();

	void SetUpdatedComponent(USceneComponent* Component) override;

	void PhysWalking(float DeltaTime, int32 Iterations) override;

	FRotator GetDeltaRotation(float DeltaTime) const override;
	void PhysicsRotation(float DeltaTime) override;

	float GetMaxSpeed() const override;

	float GetMaxWalkSpeed() const;

public:
	void SetASC(UDeckAbilitySystemComponent* ASC);

	void SetWandering(bool Wandering);

};