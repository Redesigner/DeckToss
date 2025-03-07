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

UENUM(BlueprintType)
enum EDeckPlayerStatus : uint8
{
	Alive,
	KnockedOut,
	Dead
};

UCLASS()
class DECKGAME_API ADeckPlayerState : public APlayerState, public IAbilitySystemInterface, public ICardDeckInterface
{
	GENERATED_BODY()
	
	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDeckAbilitySystemComponent> AbilitySystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDeckAbilitySet>> AbilitySets;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = Attributes, Meta = (AllowPrivateAccess))
	TObjectPtr<UBaseAttributeSet> AttributeSet;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Behavior, meta = (AllowPrivateAccess = true))
	//TEnumAsByte<EDeckTeam> TeamId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UCardDeckComponent> CardDeck;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Lives, Meta = (AllowPrivateAccess))
	uint8 LivesCount = 3;

	/// How many lives the player starts with on a new save or after a game over state
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Lives, Meta = (AllowPrivateAccess))
	uint8 StartingLivesCount = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Lives, Meta = (AllowPrivateAccess))
	TEnumAsByte<EDeckPlayerStatus> Status = EDeckPlayerStatus::Alive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Lives, Meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float KnockOutTime = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Lives, Meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float RespawnTime = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Lives, Meta = (AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> KnockedDownEffect;

public:
	UFUNCTION(BlueprintCallable, Category = "Deck|PlayerState")
	UDeckAbilitySystemComponent* GetDeckAbilitySystemComponent() const { return AbilitySystem; }

	UFUNCTION(BlueprintCallable, Category = "Deck|PlayerState")
	UBaseAttributeSet* GetAttributeSet() const;

	void GrantAbilities();

	/// Set our attributes to their default values;
	void InitializeAttributes();

	void OnAttributeSetDeath(FGameplayEffectSpec SpecCauser);

	bool IsAlive() const { return Status != EDeckPlayerStatus::Dead; }

	DECLARE_MULTICAST_DELEGATE(FOnDeath)
	FOnDeath OnDeath;

	DECLARE_MULTICAST_DELEGATE(FOnKnockedOut)
	FOnKnockedOut OnKnockedOut;

	DECLARE_MULTICAST_DELEGATE(FOnRespawn)
	FOnRespawn OnRespawn;

	UCardDeckComponent* GetCardDeckComponent() const override;

private:
	bool bReceivingNotifications = false;

	bool bAbilitiesGranted = false;

	FTimerHandle KnockOutTimer;
	FTimerHandle RespawnTimer;

	TQueue<FDeckMessage> UnprocessedNotifications;

	ADeckPlayerState();
	
	void BeginPlay() override;

	void FlushNotifications();

	void Die();

	void Respawn();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
