// Copyright (c) 2025 Stephen Melnick

#include "Character/DeckPlayerState.h"

#include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Ability/DeckAbilitySystemComponent.h"
#include "Ability/DeckAbilitySet.h"
#include "Ability/Attributes/BaseAttributeSet.h"
#include "Character/DeckPlayerController.h"
#include "Character/Components/CardDeckComponent.h"

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
    Status = Alive;
}

void ADeckPlayerState::StartRevival(UAbilitySystemComponent* Reviver)
{
    GetWorldTimerManager().PauseTimer(KnockoutTimer);
}

void ADeckPlayerState::StopRevival(UAbilitySystemComponent* Reviver)
{
    GetWorldTimerManager().UnPauseTimer(KnockoutTimer);
}

void ADeckPlayerState::IncreaseRevivePower(float PowerIncrement, UAbilitySystemComponent* Reviver)
{
    CurrentRevivePower += PowerIncrement;
    if (CurrentRevivePower >= RequiredRevivePower)
    {
        Revive(Reviver);
    }
}

void ADeckPlayerState::OnAttributeSetDeath(FGameplayEffectSpec SpecCauser)
{
    const AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
    if (!GameState)
    {
        Die();
        return;
    }

    if (GameState->PlayerArray.Num() <= 1)
    {
        Die();
        return;
    }
    
    Status = KnockedOut;
    CurrentRevivePower = 0.0f;
    
    GetWorldTimerManager().SetTimer(KnockoutTimer, FTimerDelegate::CreateUObject(this, &ADeckPlayerState::Die), KnockOutTime, false);
    OnRespawnTimerStarted.Broadcast(KnockoutTimer);
    if (KnockedDownEffect)
    {
        const FGameplayEffectSpecHandle KnockdownEffectSpecHandle = AbilitySystem->MakeOutgoingSpec(KnockedDownEffect, 1.0f, AbilitySystem->MakeEffectContext());
        KnockdownEffectHandle = AbilitySystem->ApplyGameplayEffectSpecToSelf(*KnockdownEffectSpecHandle.Data.Get());
    }
    OnKnockedOut.Broadcast();
}

void ADeckPlayerState::Die()
{
    Status = EDeckPlayerStatus::Dead;
    if (LivesCount >= 1)
    {
        GetWorldTimerManager().SetTimer(RespawnTimer, FTimerDelegate::CreateUObject(this, &ADeckPlayerState::Respawn), RespawnTime, false);
    }
    AbilitySystem->RemoveAllActiveEffects();
    OnDeath.Broadcast();
    
    if (GetPawn())
    {
        GetPawn()->Destroy();
    }
}

void ADeckPlayerState::Revive(UAbilitySystemComponent* Reviver)
{
    UE_LOGFMT(LogDeckGame, Display, "DeckPlayerState: '{SelfName}' revived by player '{ReviverName}'", GetName(), Reviver ? Reviver->GetName() : "null");
    GetWorldTimerManager().ClearTimer(KnockoutTimer);
    Status = Alive;
    OnRespawnTimerStopped.Broadcast();
    OnRevived.Broadcast();
    AbilitySystem->RemoveActiveGameplayEffect(KnockdownEffectHandle);
    
    if (RevivedEffect)
    {
        const FGameplayEffectSpecHandle KnockdownEffectSpecHandle = AbilitySystem->MakeOutgoingSpec(RevivedEffect, 1.0f, AbilitySystem->MakeEffectContext());
        AbilitySystem->ApplyGameplayEffectSpecToSelf(*KnockdownEffectSpecHandle.Data.Get());
    }
}

void ADeckPlayerState::Respawn()
{
    if (LivesCount == 0)
    {
        return;
    }

    LivesCount--;
    OnLivesChanged.Broadcast(LivesCount);
    
    if (GetPawn())
    {
        GetPawn()->Destroy();
    }
    
    GetPlayerController()->ServerRestartPlayer();
    OnRespawn.Broadcast();
}
