// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffect.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ApplyEffectToOwner.generated.h"

/**
 * 
 */
UCLASS(Meta = (DisplayName = "Apply Gameplay Effect To Owner"))
class DECKGAME_API UAnimNotifyState_ApplyEffectToOwner : public UAnimNotifyState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Effect, Meta = (AllowPrivateAccess))
	TSubclassOf<UGameplayEffect> EffectClass;
	
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	FActiveGameplayEffectHandle AppliedHandle;
	
#if WITH_EDITORONLY_DATA
	bool ShouldFireInEditor() override;
#endif
};
