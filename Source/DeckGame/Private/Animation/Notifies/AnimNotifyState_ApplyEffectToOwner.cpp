// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Animation/Notifies/AnimNotifyState_ApplyEffectToOwner.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

void UAnimNotifyState_ApplyEffectToOwner::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (!EffectClass)
	{
		return;
	}
	
	if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = AbilityInterface->GetAbilitySystemComponent())
		{
			const FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
			const FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.0f, Context);
			AppliedHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}
	}
}

void UAnimNotifyState_ApplyEffectToOwner::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!AppliedHandle.IsValid())
	{
		return;
	}

	if (IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		if (UAbilitySystemComponent* AbilitySystemComponent = AbilityInterface->GetAbilitySystemComponent())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(AppliedHandle);
			AppliedHandle.Invalidate();
		}
	}
}


#if WITH_EDITORONLY_DATA
bool UAnimNotifyState_ApplyEffectToOwner::ShouldFireInEditor()
{
	return false;
}
#endif
