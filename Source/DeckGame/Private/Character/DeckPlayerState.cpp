// Copyright (c) 2024 Stephen Melnick

#include "Character/DeckPlayerState.h"

#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/DeckAbilitySet.h"
#include "Ability/Attributes/BaseAttributeSet.h"
#include "DeckGame.h"
#include "Character/DeckPlayerController.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "Logging/StructuredLog.h"


ADeckPlayerState::ADeckPlayerState()
{
    AbilitySystem = CreateDefaultSubobject<UDeckAbilitySystemComponent>(TEXT("DeckAbilitySystemComponent"));

    AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSet"));
    AttributeSet->OnDeath.AddUObject(this, &ThisClass::OnAttributeSetDeath);

    CardDeck = CreateDefaultSubobject<UCardDeckComponent>(TEXT("CardDeck"));
}


UAbilitySystemComponent* ADeckPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystem;
}

UCardDeckComponent* ADeckPlayerState::GetCardDeckComponent() const
{
    return CardDeck;
}


UBaseAttributeSet* ADeckPlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

void ADeckPlayerState::BeginPlay()
{
    if (ADeckPlayerController* DeckPlayerController = Cast<ADeckPlayerController>(GetPlayerController()))
    {
        DeckPlayerController->OnHUDCreated.AddUObject(this, &ThisClass::FlushNotifications);
    }
}

void ADeckPlayerState::FlushNotifications()
{
    FDeckMessage Notification;
    while (UnprocessedNotifications.Dequeue(Notification))
    {
        UGameplayMessageSubsystem::Get(this).BroadcastMessage(Notification.Verb, Notification);
    }
    bReceivingNotifications = true;
}


void ADeckPlayerState::GrantAbilities()
{
    if (bAbilitiesGranted)
    {
        // Don't grant default abilities again
        return;
    }

    for (const UDeckAbilitySet* AbilitySet : AbilitySets)
    {
        if (!AbilitySet)
        {
            continue;
        }
        UE_LOGFMT(LogDeckGame, Display, "AbilitySet '{AbilitySetName}' granted to player state '{PlayerStateName}'", AbilitySet->GetName(), GetName());
        AbilitySet->GiveToAbilitySystem(AbilitySystem, nullptr);
    }
    bAbilitiesGranted = true;
}

void ADeckPlayerState::InitializeAttributes()
{
    AttributeSet->SetHealth(AttributeSet->GetMaxHealth());
    bAlive = true;
}

void ADeckPlayerState::OnAttributeSetDeath(FGameplayEffectSpec SpecCauser)
{
    bAlive = false;
    
    AbilitySystem->RemoveAllActiveEffects();
    OnDeath.Broadcast();
}
