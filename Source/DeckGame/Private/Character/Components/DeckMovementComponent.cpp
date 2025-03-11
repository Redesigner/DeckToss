// Copyright (c) 2024 Stephen Melnick

#include "Character/Components/DeckMovementComponent.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/DeckGameplayTags.h"

UDeckMovementComponent::UDeckMovementComponent()
{
}

void UDeckMovementComponent::SetUpdatedComponent(USceneComponent* Component)
{
	Super::SetUpdatedComponent(Component);

	AbilitySystemComponent = Cast<UDeckAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
}

void UDeckMovementComponent::SetASC(UDeckAbilitySystemComponent* ASC)
{
	AbilitySystemComponent = ASC;
	ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::OnGameplayEffectAppliedToOwner);
	ASC->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &ThisClass::OnGameplayEffectRemovedFromOwner);
}

void UDeckMovementComponent::SetWandering(bool Wandering)
{
	bWandering = Wandering;
}

void UDeckMovementComponent::PhysWalking(float DeltaTime, int32 Iterations)
{
	if (Velocity.IsNearlyZero())
	{
		bIsSprinting = false;
	}

	if (bStunned)
	{
		return;	
	}
	
	Super::PhysWalking(DeltaTime, Iterations);
}

void UDeckMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	Super::PhysFalling(deltaTime, Iterations);
}

FRotator UDeckMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (bStunned)
	{
		return FRotator();
	}

	return Super::GetDeltaRotation(DeltaTime);
}

void UDeckMovementComponent::PhysicsRotation(float DeltaTime)
{
	//if (AbilitySystemComponent.IsValid() && AbilitySystemComponent->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Immobile))
	//{
	//	return;
	//}

	Super::PhysicsRotation(DeltaTime);
}

float UDeckMovementComponent::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
		return GetMaxWalkSpeed();
	case MOVE_Falling:
		return MaxWalkSpeed;
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_Custom:
		return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}

float UDeckMovementComponent::GetMaxWalkSpeed() const
{
	if (bIsSprinting)
	{
		return MaxWalkSpeed * SprintSpeedModifier;
	}

	return MaxWalkSpeed;
}

void UDeckMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	Super::RequestDirectMove(MoveVelocity, bForceMaxSpeed);
}

void UDeckMovementComponent::RequestPathMove(const FVector& MoveInput)
{
	Super::RequestPathMove(MoveInput);
}

bool UDeckMovementComponent::CanStartPathFollowing() const
{
	return true;
}

bool UDeckMovementComponent::CanStopPathFollowing() const
{
	return !IsFalling();
}

void UDeckMovementComponent::StopActiveMovement()
{
	Super::StopActiveMovement();
}

void UDeckMovementComponent::OnGameplayEffectAppliedToOwner(UAbilitySystemComponent* Source,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	if (Source->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Stun))
	{
		bStunned = true;
	}

	if (Source->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Frozen))
	{
		bFrozen = true;
	}
}

void UDeckMovementComponent::OnGameplayEffectRemovedFromOwner(const FActiveGameplayEffect& ActiveGameplayEffect)
{
	if (!AbilitySystemComponent->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Stun))
	{
		bStunned = false;
	}
	
	if (!AbilitySystemComponent->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Frozen))
	{
		bFrozen = false;
	}
}
