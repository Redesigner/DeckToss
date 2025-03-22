// Copyright (c) 2024 Stephen Melnick


#include "Animation/Notifies/AnimNotifyState_Hitbox.h"

#include "Animation/AnimSequenceHelpers.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

void UAnimNotifyState_Hitbox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
#if WITH_EDITOR
	if (MeshComp->GetWorld()->WorldType == EWorldType::EditorPreview)
	{
		//SpawnEditorShape(MeshComp->GetWorld(), MeshComp);
		return;
	}
#endif

	AActor* OwnerActor = MeshComp->GetOwner();
	UMeleeComponent* MeleeComponent = OwnerActor->GetComponentByClass<UMeleeComponent>();
	if (!MeleeComponent)
	{
		return;
	}

	Hitbox = MeleeComponent->SpawnHitbox(HitboxParameters);
}

void UAnimNotifyState_Hitbox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}
	
	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
	{
		return;
	}
	
	UMeleeComponent* MeleeComponent = OwnerActor->GetComponentByClass<UMeleeComponent>();
	if (!MeleeComponent)
	{
		return;
	}

	MeleeComponent->DestroyHitbox(Hitbox.Get());
}

#if WITH_EDITOR
void UAnimNotifyState_Hitbox::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
}

void UAnimNotifyState_Hitbox::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp->GetWorld()->WorldType != EWorldType::EditorPreview)
	{
		return;
	}

	/*FVector Position = Animation->ExtractRootMotion(0.0f, EventReference.GetCurrentAnimationTime(), false).TransformPosition(HitboxParameters.Position);
	switch (HitboxParameters.Shape)
	{
	case Box:
	{
		DrawDebugBox(MeshComp->GetWorld(), Position, HitboxParameters.BoxExtents, HitboxParameters.Rotation.Quaternion(), FColor::Red, false, FrameDeltaTime * 2.0f);
		break;
	}
	case Capsule:
	{
		DrawDebugCapsule(MeshComp->GetWorld(), Position, HitboxParameters.HalfHeight, HitboxParameters.Radius, HitboxParameters.Rotation.Quaternion(), FColor::Red, false, FrameDeltaTime);
		break;
	}
	case Sphere:
	{
		DrawDebugSphere(MeshComp->GetWorld(), Position, HitboxParameters.Radius, 32, FColor::Red, false, FrameDeltaTime);
		break;
	}
	default:
		break;
	}*/
}

void UAnimNotifyState_Hitbox::DrawInEditor(class FPrimitiveDrawInterface* PDI, USkeletalMeshComponent* MeshComp,
	const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const
{
	Super::DrawInEditor(PDI, MeshComp, Animation, NotifyEvent);

	float StartTime = NotifyEvent.GetTriggerTime();
	float EndTime = NotifyEvent.GetEndTriggerTime();
	const UAnimMontage* CurrentMontage = Cast<UAnimMontage>(Animation);
	float CurrentTime = CurrentMontage ? MeshComp->GetAnimInstance()->Montage_GetPosition(CurrentMontage) : -1.0f;
	FVector CurrentPosition = HitboxParameters.Position;
	if (CurrentMontage)
	{
		const FVector RootPositionOffset =  UE::Anim::ExtractRootMotionFromAnimationAsset(CurrentMontage, nullptr, 0.0f, CurrentTime).GetLocation();
		CurrentPosition += RootPositionOffset;
	}
	if (CurrentTime >= StartTime && CurrentTime <= EndTime)
	{
		// DrawDebugBox(MeshComp->GetWorld(), CurrentPosition, FVector(100.0f), FColor::Red, false, 0.1f);
		const FVector Box = HitboxParameters.BoxExtents;
		const FLinearColor Color = FColor::Red;
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X,  Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X, -Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X, -Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X, -Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X, -Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X,  Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X,  Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X,  Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X,  Box.Y, -Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X, -Box.Y, -Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X, -Box.Y, -Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X,  Box.Y, -Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X,  Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X, -Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector( Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X, -Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X,  Box.Y,  Box.Z)), CurrentPosition + HitboxParameters.Rotation.RotateVector(FVector(-Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);	}
}

void UAnimNotifyState_Hitbox::DrawCanvasInEditor(FCanvas& Canvas, FSceneView& View, USkeletalMeshComponent* MeshComp,
	const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const
{
	Super::DrawCanvasInEditor(Canvas, View, MeshComp, Animation, NotifyEvent);
}
#endif
