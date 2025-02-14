// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "DeckPlayerController.generated.h"

class ADeckPlayerState;
class UInputMappingContext;
class UDeckAbilitySystemComponent;
class UHealthViewModel;
class UUserWidget;
class UMaterialParameterCollection;

UCLASS()
class DECKGAME_API ADeckPlayerController : public APlayerController
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnPotentialAbilityActivationChanged)
	DECLARE_MULTICAST_DELEGATE(FOnHUDCreated)
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> RootWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TScriptInterface<IBlendableInterface> BlindEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UMaterialParameterCollection> EffectParameterCollection;


public:
	UFUNCTION(BlueprintCallable)
	UDeckAbilitySystemComponent* GetDeckAbilitySystemComponent() const;

	FOnPotentialAbilityActivationChanged OnPotentialAbilityActivationChanged;

	FOnHUDCreated OnHUDCreated;

	UUserWidget* GetRootWidget() const { return RootWidget.Get(); }

private:
	TWeakObjectPtr<UUserWidget> RootWidget;

	bool bIsBlind = false;

	ADeckPlayerController();

	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void BeginPlayingState() override;
	void Tick(float DeltaSeconds) override;
	void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	// Begin IDeckTeamAgentInterface
	//void OverrideTeam(EDeckTeam NewTeam) override;
	//void SetDeckTeam(EDeckTeam InTeam) override;
	//EDeckTeam GetDeckTeam() const override;
	// End IDeckTeamAgentInterface

	void PotentialInteractionsChanged();

	void SpawnHUD();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupUI(ADeckPlayerState* WitchPlayerState);
};
