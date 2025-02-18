// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "Ability/DeckAbilitySet.h"

#include "ItemHandleComponent.generated.h"

class ACardItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DECKGAME_API UItemHandleComponent : public USceneComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnHeldItemChanged);


	UPROPERTY(ReplicatedUsing = OnRep_HeldItem, VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = true))
	TObjectPtr<ACardItem> HeldItem;


	TWeakObjectPtr<ACardItem> FakePickup;

	FDeckAbilitySet_GrantedHandles GrantedHandles;


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_HeldItem(ACardItem* OldHeldItem);

	void DropAllItems();

public:
	TWeakObjectPtr<ACardItem> RequestedPickup;

	FOnHeldItemChanged OnHeldItemChanged;


	UItemHandleComponent();

	ACardItem* ConsumeItem();

	ACardItem* GetHeldItem() const { return HeldItem; }

	ACardItem* GetFakePickup() const { return FakePickup.Get(); }

	void SetFakePickup(ACardItem* Fake);

	void PickupItem(ACardItem* Item);

	bool HoldingItem() const;

	void SetHeldItemHidden(bool bHidden);
};
