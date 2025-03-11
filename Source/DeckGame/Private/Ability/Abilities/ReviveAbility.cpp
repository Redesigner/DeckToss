// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Ability/Abilities/ReviveAbility.h"

#include "AbilitySystemComponent.h"
#include "Components/ShapeComponent.h"
#include "Logging/StructuredLog.h"

#include "DeckGame.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/PlayerCharacter/PlayerCharacter.h"
#include "Character/DeckPlayerState.h"

UReviveAbility::UReviveAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UReviveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo->AvatarActor.IsValid())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	
	UShapeComponent* InteractionVolume = Cast<UShapeComponent>(ActorInfo->AvatarActor->FindComponentByTag(UShapeComponent::StaticClass(), "Interaction Volume"));
	if (!InteractionVolume)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	TArray<AActor*> OverlappingActors;
	//GetWorld()->ComponentOverlapMulti(InteractionOverlaps, InteractionVolume, InteractionVolume->GetComponentLocation(), InteractionVolume->GetComponentQuat());
	InteractionVolume->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
	if (OverlappingActors.IsEmpty())
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	ADeckPlayerState* PlayerToRevive = nullptr;
	for (AActor* OverlappingActor : OverlappingActors)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OverlappingActor);
		if (!PlayerCharacter)
		{
			continue;
		}
		
		ADeckPlayerState* PlayerState = Cast<ADeckPlayerState>(PlayerCharacter->GetPlayerState());
		if (PlayerState && PlayerState->GetStatus() == KnockedOut)
		{
			PlayerToRevive = PlayerState;
			break;
		}
	}

	if (!PlayerToRevive)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	//UE_LOGFMT(LogDeckGame, Display, "Revive ability started.");
	if (!PlayerToRevive->StartRevival(ActorInfo->AbilitySystemComponent.Get()))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	
	if (ReviveEffect)
	{
		AppliedReviveEffect = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, MakeOutgoingGameplayEffectSpec(ReviveEffect));
	}
	else
	{
		UE_LOGFMT(LogDeckGame, Warning, "Revive Ability: No gameplay effect was set for revive ability: '{AbilityName}'", GetName());
	}
	
	RevivalTarget = PlayerToRevive;
	TargetRevivedDelegateHandle = PlayerToRevive->OnRevived.AddWeakLambda(this, [this, Handle, ActorInfo, ActivationInfo]()
	{
		ThisClass::EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	});
	GetWorld()->GetTimerManager().SetTimer(ButtonPressTimer, FTimerDelegate::CreateLambda([this, Handle, ActorInfo, ActivationInfo]()
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}), MaxTimeBetweenPresses, false);


	if (ReviveAnimation)
	{
		UAbilityTask_PlayMontageAndWait* PlayReviveAnimation = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, "Play Revive Animation", ReviveAnimation);
		PlayReviveAnimation->ReadyForActivation();
	}
	else
	{
		UE_LOGFMT(LogDeckGame, Warning, "Revive Ability: No revive animation was set for revive ability: '{AbilityName}'", GetName());
	}
	
	UE_LOGFMT(LogDeckGame, Display, "Revive Ability '{AbilityName}' successfully activated.", GetName());
}

void UReviveAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (ButtonPressTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(ButtonPressTimer);
	}
	
	if (bWasCancelled)
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		//UE_LOGFMT(LogDeckGame, Display, "Revive ability cancelled.");
		return;
	}
	
	//UE_LOGFMT(LogDeckGame, Display, "Revive ability ended.");
	if (AppliedReviveEffect.IsValid())
	{
		if (ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(AppliedReviveEffect))
		{
			//UE_LOGFMT(LogDeckGame, Display, "Revive ability: Removing gameplay effect");
		}
	}
	if (RevivalTarget.IsValid())
	{
		RevivalTarget->OnRevived.Remove(TargetRevivedDelegateHandle);
		TargetRevivedDelegateHandle.Reset();
		RevivalTarget->StopRevival(ActorInfo->AbilitySystemComponent.Get());
		RevivalTarget = nullptr;
	}

	ActorInfo->AbilitySystemComponent->FindAbilitySpecFromHandle(Handle)->InputPressed = false;
}

void UReviveAbility::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	UE_LOGFMT(LogDeckGame, Display, "Revive ability pressed again.");
	GetWorld()->GetTimerManager().SetTimer(ButtonPressTimer, FTimerDelegate::CreateLambda([this, Handle, ActorInfo, ActivationInfo]()
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}), MaxTimeBetweenPresses, false);

	if (!RevivalTarget.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	RevivalTarget->IncreaseRevivePower(1.0f, ActorInfo->AbilitySystemComponent.Get());
}

bool UReviveAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}
	
	UShapeComponent* InteractionVolume = Cast<UShapeComponent>(ActorInfo->AvatarActor->FindComponentByTag(UShapeComponent::StaticClass(), "Interaction Volume"));
	if (!InteractionVolume)
	{
		return false;
	}

	TArray<AActor*> OverlappingActors;
	InteractionVolume->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
	if (OverlappingActors.IsEmpty())
	{
		return false;
	}

	ADeckPlayerState* PlayerToRevive = nullptr;
	for (AActor* OverlappingActor : OverlappingActors)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OverlappingActor);
		if (!PlayerCharacter)
		{
			continue;
		}
		
		ADeckPlayerState* PlayerState = Cast<ADeckPlayerState>(PlayerCharacter->GetPlayerState());
		if (PlayerState && PlayerState->GetStatus() == KnockedOut)
		{
			PlayerToRevive = PlayerState;
			break;
		}
	}

	return PlayerToRevive != nullptr;
}
