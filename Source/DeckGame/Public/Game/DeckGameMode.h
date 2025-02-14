// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h" 

#include "DeckGameMode.generated.h"

class UItemSet;
class UPotionRecipeSet;
class UBestiaryData;
class UGameplayEffectDataSet;

UCLASS()
class DECKGAME_API ADeckGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ADeckGameMode();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:
	void PostLogin(APlayerController* NewPlayer) override;

	void StartPlay() override;
};