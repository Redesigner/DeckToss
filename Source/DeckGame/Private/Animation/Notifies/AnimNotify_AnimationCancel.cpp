// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Animation/Notifies/AnimNotify_AnimationCancel.h"

#include "AbilitySystemInterface.h"
#include "Ability/DeckAbilitySystemComponent.h"


UAnimNotify_AnimationCancel::UAnimNotify_AnimationCancel()
{
}

bool UAnimNotify_AnimationCancel::ShouldFireInEditor()
{
	return false;
}

void UAnimNotify_AnimationCancel::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	const IAbilitySystemInterface* OwnerActor = Cast<IAbilitySystemInterface>(MeshComp->GetOwner());
	if (!OwnerActor)
	{
		return;
	}

	if (UDeckAbilitySystemComponent* ASC = Cast<UDeckAbilitySystemComponent>(OwnerActor->GetAbilitySystemComponent()))
	{
		ASC->bCanAnimationBeCancelled = true;
	}
}
