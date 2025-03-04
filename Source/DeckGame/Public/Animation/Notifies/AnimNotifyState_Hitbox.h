// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "Character/Components/MeleeComponent.h"

#include "AnimNotifyState_Hitbox.generated.h"


class UShapeComponent;

UCLASS()
class DECKGAME_API UAnimNotifyState_Hitbox : public UAnimNotifyState
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shape, meta = (AllowPrivateAccess = true))
	FMeleeHitboxSpawnParameters HitboxParameters;

	TWeakObjectPtr<UShapeComponent> Hitbox;


	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// void SpawnEditorShape(UWorld* World, USkeletalMeshComponent* MeshComp);

	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
#endif
};
