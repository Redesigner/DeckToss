// Copyright (c) 2024 Stephen Melnick


#include "Character/DeckPlayerController.h"

#include "Character/DeckPlayerState.h"
#include "Character/PlayerCharacter/PlayerCharacter.h"
#include "Game/DeckGameMode.h"
#include "Game/DeckCheatManager.h"
#include "Ability/DeckGameplayTags.h"
#include "Ability/DeckAbilitySystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Camera/CameraComponent.h"

ADeckPlayerController::ADeckPlayerController()
{
    CheatClass = UDeckCheatManager::StaticClass();
}


void ADeckPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (ADeckPlayerState* WitchPlayerState = GetPlayerState<ADeckPlayerState>())
    {
        WitchPlayerState->GrantAbilities();
        WitchPlayerState->InitializeAttributes();

        if (HasAuthority() && IsLocalPlayerController())
        {
            SpawnHUD();
        }
    }
}

void ADeckPlayerController::BeginPlayingState()
{
    Super::BeginPlayingState();

    if (!IsLocalPlayerController())
    {
        return;
    }

    if (ADeckPlayerState* DeckPlayerState = GetPlayerState<ADeckPlayerState>())
    {
        SetupUI(DeckPlayerState);

        // We don't need to wait for our attributes to be set up, if this isn't an authoratative controller
        if (!HasAuthority())
        {
            SpawnHUD();
            OnHUDCreated.Broadcast();
        }
    }
}

void ADeckPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bIsBlind || !EffectParameterCollection.IsValid() || !GetPawn())
    {
        return;
    }

    if (!IsLocalPlayerController())
    {
        return;
    }

    UMaterialParameterCollectionInstance* CollectionInstance = GetWorld()->GetParameterCollectionInstance(EffectParameterCollection.Get());
    if (!CollectionInstance)
    {
        return;
    }

    CollectionInstance->SetVectorParameterValue("Center", GetPawn()->GetActorLocation());
}

void ADeckPlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (!InputMapping.IsNull())
            {
                InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 1);
            }
        }
    }
}

UDeckAbilitySystemComponent* ADeckPlayerController::GetDeckAbilitySystemComponent() const
{
    if (ADeckPlayerState* WitchPlayerState = GetPlayerState<ADeckPlayerState>())
    {
        return WitchPlayerState->GetDeckAbilitySystemComponent();
    }

    return nullptr;
}


void ADeckPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
    Super::PostProcessInput(DeltaTime, bGamePaused);

    UDeckAbilitySystemComponent* DeckAbilitySystemComponent = GetDeckAbilitySystemComponent();
    if (!DeckAbilitySystemComponent)
    {
        return;
    }

    DeckAbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
}

void ADeckPlayerController::PotentialInteractionsChanged()
{
    OnPotentialAbilityActivationChanged.Broadcast();
}

void ADeckPlayerController::SpawnHUD()
{
    // Make sure we don't create another rootwidget
    if (RootWidgetClass && !RootWidget.IsValid())
    {
        RootWidget = CreateWidget<UUserWidget>(this, RootWidgetClass.Get());
        RootWidget->AddToViewport();
    }
}