// Copyright (c) 2025 Stephen Melnick

#pragma once

#include "AbilitySystemInterface.h"
#include "CoreMinimal.h"
#include "DeckTeamAgentInterface.h"
#include "GameplayEffect.h"
#include "GameFramework/PlayerState.h"

#include "Character/Components/CardDeckInterface.h"
#include "Game/DeckMessage.h"

#include "DeckPlayerState.generated.h"

class UStateTreeComboComponent;
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
class DECKGAME_API ADeckPlayerState : public APlayerState,
	public IAbilitySystemInterface, public ICardDeckInterface, public IDeckTeamAgentInterface
{
	GENERATED_BODY()
	
	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UDeckAbilitySystemComponent> AbilitySystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities, Meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDeckAbilitySet>> AbilitySets;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = Attributes, Meta = (AllowPrivateAccess))
	TObjectPtr<UBaseAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UCardDeckComponent> CardDeck;

	UPROPERTY(Instanced, VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UStateTreeComboComponent> ComboComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Team, Meta = (AllowPrivateAccess))
	TEnumAsByte<EDeckTeam> TeamId = EDeckTeam::Friendly;

	//--------------------Lives and Knockdown------------------
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Lives, Meta = (AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> RevivedEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Lives, Meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float RequiredRevivePower = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Lives, Meta = (AllowPrivateAccess))
	float CurrentRevivePower = 0.0f;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Deck|PlayerState")
	UDeckAbilitySystemComponent* GetDeckAbilitySystemComponent() const { return AbilitySystem; }

	UFUNCTION(BlueprintCallable, Category = "Deck|PlayerState")
	UBaseAttributeSet* GetAttributeSet() const;

	void GrantAbilities();

	/// Set our attributes to their default values;
	void InitializeAttributes();

	bool StartRevival(UAbilitySystemComponent* Reviver);

	void StopRevival(UAbilitySystemComponent* Reviver);

	void IncreaseRevivePower(float PowerIncrement, UAbilitySystemComponent* Reviver);

	void OnAttributeSetDeath(FGameplayEffectSpec SpecCauser);

	bool IsAlive() const { return Status != EDeckPlayerStatus::Dead; }

	UFUNCTION(BlueprintCallable)
	EDeckPlayerStatus GetStatus() const { return Status; }

	uint8 GetLivesCount() const { return  LivesCount; }

	TMulticastDelegate<void()> OnDeath;

	TMulticastDelegate<void()> OnKnockedOut;

	TMulticastDelegate<void()> OnRespawn;

	TMulticastDelegate<void()> OnRevived;

	// @TODO: Move this away from delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRespawnTimerStarted, const FTimerHandle, RespawnTimerHandle);
	UPROPERTY(BlueprintAssignable)
	FOnRespawnTimerStarted OnRespawnTimerStarted;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespawnTimerStopped);
	UPROPERTY(BlueprintAssignable)
	FOnRespawnTimerStopped OnRespawnTimerStopped;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLivesChanged, uint8);
	FOnLivesChanged OnLivesChanged;
	
	UCardDeckComponent* GetCardDeckComponent() const override;

	UStateTreeComboComponent* GetComboComponent() const;
	
	void SetDeckTeam(EDeckTeam InTeam) override;
	EDeckTeam GetDeckTeam() const override;

private:
	bool bReceivingNotifications = false;

	bool bAbilitiesGranted = false;

	FActiveGameplayEffectHandle KnockdownEffectHandle;
	
	FTimerHandle KnockoutTimer;
	FTimerHandle RespawnTimer;

	TQueue<FDeckMessage> UnprocessedNotifications;

	ADeckPlayerState();
	
	void BeginPlay() override;

	void FlushNotifications();

	void Die();

	void Revive(UAbilitySystemComponent* Reviver);

	void Respawn();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
