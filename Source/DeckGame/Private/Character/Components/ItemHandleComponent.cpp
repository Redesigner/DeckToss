// Copyright (c) 2024 Stephen Melnick


#include "Character/Components/ItemHandleComponent.h"

#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

#include "DeckGame.h"
#include "Character/DeckPlayerState.h"
#include "Character/PlayerCharacter/PlayerCharacter.h"
#include "GameObjects/CardItem.h"


UItemHandleComponent::UItemHandleComponent()
{
	SetIsReplicatedByDefault(true);
}

void UItemHandleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HeldItem);
}

void UItemHandleComponent::OnRep_HeldItem(ACardItem* OldHeldItem)
{
	if (HeldItem == OldHeldItem)
	{
		return;
	}

	// Check if we need to drop our old item -- either we're carrying the wrong item,
	// or we've just replicated that we aren't actually holding an item
	if (OldHeldItem)
	{
		// if ((HeldItem.IsValid() && HeldItem != OldHeldItem) || !HeldItem.IsValid())
		{
			UE_LOGFMT(LogDeckGame, Display, "ItemHandleComponent '{SelfName}' in '{ActorName}' Held Item Replication: previously held item '{ItemName}' was incorrect, dropping item.", GetName(), GetOwner()->GetName(), OldHeldItem->GetName());

			OldHeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			OldHeldItem->bHeld = false;
			OldHeldItem->EnableMovement();
		}
	}

	// Check if we need to pick up a new item -- we have found out through replication
	// that we are carrying the wrong item, or we thought we were carrying nothing
	// but are actually carrying a new item
	if (HeldItem)
	{
		if ((OldHeldItem && OldHeldItem != HeldItem) || !OldHeldItem)
		{
			UE_LOGFMT(LogDeckGame, Display, "ItemHandleComponent '{SelfName}' in '{ActorName}' Held Item Replication: previously held item '{ItemName}' was incorrect, picking up item '{NewItemname}'.",
				GetName(), GetOwner()->GetName(), OldHeldItem ? OldHeldItem->GetName() : "None", HeldItem->GetName());

			HeldItem->DisableMovement();
			HeldItem->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			HeldItem->bHeld = true;
		}
	}
}

void UItemHandleComponent::DropAllItems()
{
	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);
	UE_LOGFMT(LogDeckGame, Display, "ItemHandleComponent '{SelfName}' in '{ActorName}' was holding {ItemCount} items.", GetName(), GetOwner()->GetName(), AttachedActors.Num());
	for (AActor* AttachedActor : AttachedActors)
	{
		if (ACardItem* AttachedCard = Cast<ACardItem>(AttachedActor))
		{
			AttachedCard->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			AttachedCard->bHeld = false;
			AttachedCard->EnableMovement();
		}
	}
}

void UItemHandleComponent::SetFakePickup(ACardItem* Fake)
{
	if (FakePickup.IsValid())
	{
		FakePickup->Destroy();
	}

	FakePickup = Fake;
}

ACardItem* UItemHandleComponent::ConsumeItem()
{
	if (!HeldItem)
	{
		return nullptr;
	}

	ACardItem* Item = HeldItem.Get();
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item->bHeld = false;
	HeldItem = nullptr;

	Item->EnableMovement();
	OnHeldItemChanged.Broadcast();
	return Item;
}

void UItemHandleComponent::PickupItem(ACardItem* Item)
{
	if (HeldItem)
	{
		return;
	}

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner)
	{
		return;
	}

	HeldItem = Item;
	Item->DisableMovement();
	Item->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Item->bHeld = true;

	OnHeldItemChanged.Broadcast();
}

bool UItemHandleComponent::HoldingItem() const
{
	return HeldItem != nullptr;
}

void UItemHandleComponent::SetHeldItemHidden(bool bHidden)
{
	if (HeldItem)
	{
		HeldItem->SetActorHiddenInGame(bHidden);
	}
}
