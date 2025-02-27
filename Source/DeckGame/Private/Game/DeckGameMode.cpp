// Copyright (c) 2024 Stephen Melnick

#include "Game/DeckGameMode.h"

#include "DeckGame.h"
#include "Kismet/GameplayStatics.h"

ADeckGameMode::ADeckGameMode()
{
}

void ADeckGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
}


void ADeckGameMode::StartPlay()
{
    Super::StartPlay();
}

void ADeckGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    UGameplayStatics::CreatePlayer(this);
}

UCardAbilityMap* ADeckGameMode::GetCards() const
{
    return Cards;
}
