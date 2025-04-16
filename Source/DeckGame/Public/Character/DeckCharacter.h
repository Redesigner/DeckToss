// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DeckCharacter.generated.h"

UCLASS()
class DECKGAME_API ADeckCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	void TeleportToStart();
	
	void Tick(float DeltaSeconds) override;

	void BeginPlay() override;
	
	FVector LastSafeLocation;
};
