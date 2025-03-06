// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Character/Enemies/DeckEnemy.h"

#include "GameplayEffect.h"

#include "Ability/Attributes/BaseAttributeSet.h"
#include "Ability/DeckAbilitySet.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Character/Components/DeckMovementComponent.h"


ADeckEnemy::ADeckEnemy(const FObjectInitializer& ObjectInitializer) :
	ACharacter(ObjectInitializer.SetDefaultSubobjectClass(ACharacter::CharacterMovementComponentName, UDeckMovementComponent::StaticClass()))
{
	AbilitySystemComponent = CreateDefaultSubobject<UDeckAbilitySystemComponent>(TEXT("AbilitySystem"));
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("Base Attribute Set"));
	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::GameplayEffectApplied);
}

void ADeckEnemy::MoveForward()
{
	AddMovementInput(GetActorForwardVector());
}

void ADeckEnemy::BeginPlay()
{
	for (UDeckAbilitySet* AbilitySet : AbilitySets)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
	}

	AttributeSet->OnDeath.AddUObject(this, &ThisClass::OnDeath);
	Super::BeginPlay();
}

UAbilitySystemComponent* ADeckEnemy::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADeckEnemy::GameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	if (!AttributeSet)
	{
		return;
	}

	for (const FGameplayEffectModifiedAttribute& ModifiedAttribute : GameplayEffectSpec.ModifiedAttributes)
	{
		if (!bAlive)
		{
			return;
		}

		if (ModifiedAttribute.Attribute == AttributeSet->GetDamageAttribute())
		{
			AActor* Damager = GameplayEffectSpec.GetContext().GetInstigator();
			if (GameplayEffectSpec.GetContext().GetHitResult())
			{
				FHitResult Hit = *GameplayEffectSpec.GetContext().GetHitResult();
				OnTakeDamage.Broadcast(Damager, Hit);
			}
			else
			{
				OnTakeDamage.Broadcast(Damager, FHitResult());
			}
		}
	}
}

void ADeckEnemy::OnDeath(const FGameplayEffectSpec EffectSpec)
{
	UE_LOG(LogTemp, Display, TEXT("Enemy '%s' died."), *GetName());
	bAlive = false;
	// DropTableComponent->DropItems();
	AbilitySystemComponent->CancelAllAbilities();
	SetLifeSpan(2.0f);
	if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		MovementComponent->SetMovementMode(EMovementMode::MOVE_None);
	}
}
