// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/PlayerState.h"

#include "Character/Components/CardDeckInterface.h"
#include "Game/DeckMessage.h"

#include "DeckPlayerState.generated.h"

class UBaseAttributeSet;
class UCardDeckComponent;
class UDeckAbilitySet;
class UDeckAbilitySystemComponent;

UCLASS()
class DECKGAME_API ADeckPlayerState : public APlayerState, public IAbilitySystemInterface, public ICardDeckInterface
{
	GENERATED_BODY()
	
	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDeckAbilitySystemComponent> AbilitySystem;

	// This should be set whenever the pawn type changes, but for now, this should be fine
	// It is also currently only loaded on construction, but should be able to change in the future
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDeckAbilitySet>> AbilitySets;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBaseAttributeSet> AttributeSet;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Behavior, meta = (AllowPrivateAccess = true))
	//TEnumAsByte<EDeckTeam> TeamId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UCardDeckComponent> CardDeck;

public:
	UFUNCTION(BlueprintCallable, Category = "Deck|PlayerState")
	UDeckAbilitySystemComponent* GetDeckAbilitySystemComponent() const { return AbilitySystem; }

	UFUNCTION(BlueprintCallable, Category = "Deck|PlayerState")
	UBaseAttributeSet* GetAttributeSet() const;

	void GrantAbilities();

	/// Set our attributes to their default values;
	void InitializeAttributes();

	void OnAttributeSetDeath(FGameplayEffectSpec SpecCauser);

	bool IsAlive() const { return bAlive; }

	DECLARE_MULTICAST_DELEGATE(FOnDeath)
	FOnDeath OnDeath;

	UCardDeckComponent* GetCardDeckComponent() const override;

private:
	bool bAlive = false;

	bool bReceivingNotifications = false;

	bool bAbilitiesGranted = false;

	TQueue<FDeckMessage> UnprocessedNotifications;

	ADeckPlayerState();
	
	void BeginPlay() override;

	void FlushNotifications();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
