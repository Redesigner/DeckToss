// Copyright (c) 2024 Stephen Melnick

#include "Character/Components/DeckMovementComponent.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemGlobals.h"

#include "Ability/DeckAbilitySystemComponent.h"

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
}

void UDeckMovementComponent::SetWandering(bool Wandering)
{
	bWandering = Wandering;
}

void UDeckMovementComponent::PhysWalking(float DeltaTime, int32 Iterations)
{
	// Should we be doing this cast ever frame for every actor?
	if (GetOwner())
	{
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());
		//if (ASI && ASI->GetAbilitySystemComponent() && ASI->GetAbilitySystemComponent()->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Immobile))
		//{
		//	return;
		//}
	}

	if (Velocity.IsNearlyZero())
	{
		bIsSprinting = false;
	}

	Super::PhysWalking(DeltaTime, Iterations);
}

FRotator UDeckMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (GetOwner())
	{
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());
		//if (ASI && ASI->GetAbilitySystemComponent() && ASI->GetAbilitySystemComponent()->HasMatchingGameplayTag(DeckGameplayTags::GameplayEffect_Immobile))
		//{
		//	return FRotator();
		//}
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