// Copyright (c) 2024 Stephen Melnick


#include "Ability/DeckGameplayAbility.h"

#include "AIController.h"


void UDeckGameplayAbility::ActivateAbilityFailed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, int16 PredictionKey)
{
}

void UDeckGameplayAbility::ActivateAbilitySucceed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FPredictionKey& PredictionKey)
{
}

bool UDeckGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

UBlackboardComponent* UDeckGameplayAbility::GetOwnerBlackboard(bool& bBlackboardFound)
{
	if (!CurrentActorInfo->AvatarActor.IsValid())
	{
		bBlackboardFound = false;
		return nullptr;
	}

	APawn* OwningPawn = Cast<APawn>(CurrentActorInfo->AvatarActor);
	if (!OwningPawn)
	{
		bBlackboardFound = false;
		return nullptr;
	}

	AAIController* AIController = Cast<AAIController>(OwningPawn->GetController());
	if (!AIController)
	{
		bBlackboardFound = false;
		return nullptr;
	}

	if (AIController->GetBlackboardComponent())
	{
		bBlackboardFound = true;
		return AIController->GetBlackboardComponent();
	}

	bBlackboardFound = false;
	return nullptr;
}
