// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h" 

#include "DeckGameMode.generated.h"

class UBestiaryData;
class UGameplayEffectDataSet;
class UItemSet;
class UCardAbilityMap;

UCLASS()
class DECKGAME_API ADeckGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ADeckGameMode();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	UCardAbilityMap* GetCards() const;

private:
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UCardAbilityMap> Cards;

	void PostLogin(APlayerController* NewPlayer) override;

	void StartPlay() override;
};