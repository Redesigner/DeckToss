// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AnimationCancel.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API UAnimNotify_AnimationCancel : public UAnimNotify
{
	GENERATED_BODY()

	UAnimNotify_AnimationCancel();

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#if WITH_EDITORONLY_DATA
	bool ShouldFireInEditor() override;
#endif
};
