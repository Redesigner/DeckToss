// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"

#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UCardDeckComponent;
class UDeckAbilitySystemComponent;
class UDeckInputConfig;
class UInputAction;
class UInventoryComponent;
class UMeleeComponent;
class USphereComponent;
class USpringArmComponent;
struct FInputActionInstance;

UCLASS()
class DECKGAME_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayCueInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess))
	bool bTopDownCamera = true;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess))
	TObjectPtr<UDeckInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<USphereComponent> InteractionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UCardDeckComponent> CardDeck;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess))
	TObjectPtr<UMeleeComponent> Melee;

	FVector LastSafeLocation;


	DECLARE_MULTICAST_DELEGATE(FOnPotentialInteractionsChanged);


	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	void BindActions(UInputComponent* PlayerInputComponent);

	void Tick(float DeltaSeconds) override;
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void PossessedBy(AController* NewController) override;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);

	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	// Native Actions
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);

	UFUNCTION(BlueprintCallable)
	UDeckAbilitySystemComponent* GetDeckAbilitySystemComponent() const;

	void TeleportToStart();

	UFUNCTION()
	void InteractVolumeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void InteractVolumeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RotateTopDownCamera(FVector2D Input);
	void RotateOTSCamera(FVector2D Input);


public:
	// Consider making Interaction Volume a seprate component so we can use GetComponentByClass instead
	USphereComponent* GetInteractionVolume() const;

	/// Set the player's color. Must be implemented in blueprint, since mesh refs are difficult
	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerColor(FColor Color);

	void TeleportToLastSafeLocation();

	FOnPotentialInteractionsChanged OnPotentialInteractionsChanged;
};