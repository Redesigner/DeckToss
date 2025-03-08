// Copyright (c) 2024 Stephen Melnick

#include "Game/DeckGameMode.h"

#include "DeckGame.h"
#include "Character/DeckPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"

ADeckGameMode::ADeckGameMode()
{
}

void ADeckGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (ADeckPlayerState* DeckPlayerState = Cast<ADeckPlayerState>(NewPlayer->PlayerState))
    {
        DeckPlayerState->OnDeath.AddUObject(this, &ADeckGameMode::OnPlayerDeath);
    }
}


void ADeckGameMode::StartPlay()
{
    Super::StartPlay();
}

void ADeckGameMode::OnPlayerDeath()
{
    int SharedPlayerLivesCount = 0;
    for (APlayerState* PlayerState : GameState->PlayerArray)
    {
        if (ADeckPlayerState* DeckPlayerState = Cast<ADeckPlayerState>(PlayerState))
        {
            SharedPlayerLivesCount += DeckPlayerState->GetLivesCount();
        }
    }

    if (SharedPlayerLivesCount == 0)
    {
        UE_LOGFMT(LogDeckGame, Display, "Game ended.");
        GameOver();
    }
}

void ADeckGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    for (int i = 1; i < PlayerCount; ++i)
    {
        UGameplayStatics::CreatePlayer(this);
    }
}

UCardAbilityMap* ADeckGameMode::GetCards() const
{
    return Cards;
}
