// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "Character/Components/MeleeComponent.h"

#include "AnimNotifyState_Hitbox.generated.h"


class UShapeComponent;

UCLASS(meta = (DisplayName = "Spawn Hitbox"))
class DECKGAME_API UAnimNotifyState_Hitbox : public UAnimNotifyState
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shape, Meta = (AllowPrivateAccess))
	FMeleeHitboxSpawnParameters HitboxParameters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shape, Meta = (AllowPrivateAccess))
	FColor HitboxDisplayColor = FColor::Red;
	
	TWeakObjectPtr<UShapeComponent> Hitbox;


	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	// void SpawnEditorShape(UWorld* World, USkeletalMeshComponent* MeshComp);

	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void DrawInEditor(FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp, const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const override;
	virtual void DrawCanvasInEditor(FCanvas& Canvas, FSceneView& View, USkeletalMeshComponent* MeshComp, const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const override;

	/// These functions are identical to those found in draw debug shapes, but working with the PDI instead of a linebatcher
	static void DrawBox(FPrimitiveDrawInterface* PDI, FVector CurrentPosition, FVector Box, FRotator Rotation, FLinearColor Color);
	static void DrawSphereHitbox(FPrimitiveDrawInterface* PDI, FVector CurrentPosition, float Radius, FLinearColor Color);
	static void DrawCapsuleHitbox(FPrimitiveDrawInterface* PDI, FVector CurrentPosition, float Radius, float HalfHeight, FRotator Rotation, FLinearColor Color);
	static void DrawHalfCircle(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X, const FVector& Y, const FLinearColor& Color, const float Radius, int32 NumSides);
	static void DrawCircle(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X, const FVector& Y, const FLinearColor& Color, const float Radius, int32 NumSides);
#endif
};
