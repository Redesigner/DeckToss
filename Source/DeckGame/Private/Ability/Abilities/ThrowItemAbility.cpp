// Copyright (c) 2024 Stephen Melnick


#include "Ability/Abilities/ThrowItemAbility.h"

#include "GameFramework/PlayerState.h"
#include "Logging/StructuredLog.h"
#include "GameFramework/MovementComponent.h" 

#include "DeckGame.h"
#include "Ability/DeckGameplayTags.h"
#include "Character/Components/ItemHandleComponent.h"
#include "GameObjects/CardItem.h"

UThrowItemAbility::UThrowItemAbility()
{
	bConsumeInput = true;

	// For now, this ability is not triggered by holding throw!
	// This is a legacy feature from WitchForest
	// 
	//if (HasAnyFlags(RF_ClassDefaultObject))
	//{
	//	// Add the ability trigger tag as default to the CDO.
	//	FAbilityTriggerData TriggerData;
	//	TriggerData.TriggerTag = DeckGameplayTags::GameplayEvent_AbilityTrigger_Throw;
	//	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	//	AbilityTriggers.Add(TriggerData);
	//}
}

void UThrowItemAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor);
	if (!Pawn)
	{
		UE_LOGFMT(LogDeckGame, Warning, "ThrowItemAbility failed. Could not find pawn.");
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UItemHandleComponent* ItemHandle = Pawn->GetComponentByClass<UItemHandleComponent>();
	if (!ItemHandle)
	{
		UE_LOGFMT(LogDeckGame, Warning, "ThrowItemAbility failed. '{PawnName}' does not have an ItemHandleComponent.", Pawn->GetName());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (ItemHandle->HoldingItem())
	{
		ThrowItem(ItemHandle->GetHeldItem(), Pawn, ItemHandle, TriggerEventData ? TriggerEventData->EventMagnitude : 0.5f);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
	else
	{
		UE_LOGFMT(LogDeckGame, Warning, "ThrowItemAbility failed. Unable to consume pickup.");
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

bool UThrowItemAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor);
	if (!Pawn)
	{
		return false;
	}

	UItemHandleComponent* ItemHandleComponent = Pawn->GetComponentByClass<UItemHandleComponent>();
	if (!ItemHandleComponent)
	{
		return false;
	}
	return ItemHandleComponent->HoldingItem();
}


void UThrowItemAbility::ActivateAbilityFailed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, int16 PredictionKey)
{
	APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor);
	if (!Pawn)
	{
		return;
	}

	UItemHandleComponent* ItemHandleComponent = Pawn->GetComponentByClass<UItemHandleComponent>();
	if (!ItemHandleComponent)
	{
		return;
	}

	ItemHandleComponent->SetHeldItemHidden(false);

	if (!ItemHandleComponent->GetFakePickup())
	{
		return;
	}
	ItemHandleComponent->GetFakePickup()->Destroy();
}


void UThrowItemAbility::ActivateAbilitySucceed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FPredictionKey& PredictionKey)
{
	APawn* Pawn = Cast<APawn>(ActorInfo->AvatarActor);
	if (!Pawn)
	{
		return;
	}

	UItemHandleComponent* ItemHandleComponent = Pawn->GetComponentByClass<UItemHandleComponent>();
	if (!ItemHandleComponent)
	{
		return;
	}

	if (ACardItem* HeldItem = ItemHandleComponent->ConsumeItem())
	{
		// HeldItem->SetActorHiddenInGame(false);
		HeldItem->SetOwner(nullptr);
		HeldItem->SetLastHeldAbilitySystemComponent(ActorInfo->AbilitySystemComponent.Get());

		if (ItemHandleComponent->GetFakePickup())
		{
			ItemHandleComponent->GetFakePickup()->AttachFakeTo(HeldItem);
		}
		else
		{
			UE_LOGFMT(LogDeckGame, Display, "ThrowAbility '{AbilityName}' succeeded in replication, but there was no real thrown item to attach the fake to.", GetName());
		}
	}
}


void UThrowItemAbility::ThrowItem(ACardItem* Item, APawn* Pawn, UItemHandleComponent* ItemHandle, float Strength)
{
	if (IsPredictingClient())
	{
		SimulateThrowItem(Item, Pawn, ItemHandle, Strength);
	}
	else
	{
		ServerThrowItem(Item, Pawn, ItemHandle, Strength);
	}
}


void UThrowItemAbility::SimulateThrowItem(ACardItem* Item, APawn* Pawn, UItemHandleComponent* ItemHandle, float Strength)
{
	ACardItem* FakePickup = GetWorld()->SpawnActorDeferred<ACardItem>(Item->GetClass(), Item->GetTransform());
	if (!FakePickup)
	{
		UE_LOGFMT(LogDeckGame, Error, "ThrowItemAbility '{AbilityName}' failed to spawn fake pickup actor from template pickup '{PickupName}'.", GetName(), Item->GetName());
		return;
	}

	const float ThrowSpeed = MaxThrowSpeed * Strength;
	FakePickup->DisableReplication();
	FakePickup->bHeld = false;
	FakePickup->SetActorLocation(ItemHandle->GetComponentLocation());
	FakePickup->FinishSpawning(Item->GetActorTransform());
	FakePickup->SetVelocity(Pawn->GetActorForwardVector() * ThrowSpeed);
	FakePickup->SetLastHeldAbilitySystemComponent(GetAbilitySystemComponentFromActorInfo());

	if (ThrowSpeed >= MaxThrowSpeed / 2.0f)
	{
		FakePickup->SetThrown(true);
	}

	ItemHandle->SetFakePickup(FakePickup);
	ItemHandle->SetHeldItemHidden(true);
}


void UThrowItemAbility::ServerThrowItem(ACardItem* Item, APawn* Pawn, UItemHandleComponent* ItemHandle, float Strength)
{
	if (ACardItem* HeldItem = ItemHandle->ConsumeItem())
	{
		const float ThrowSpeed = MaxThrowSpeed * Strength;

		HeldItem->SetLastHeldAbilitySystemComponent(GetAbilitySystemComponentFromActorInfo());
		HeldItem->SetVelocity(Pawn->GetActorForwardVector() * ThrowSpeed);


		if (ThrowSpeed >= MaxThrowSpeed / 2.0f)
		{
			HeldItem->SetThrown(true);
		}
	}
}
