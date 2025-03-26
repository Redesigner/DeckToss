// Copyright (c) 2024 Stephen Melnick


#include "Animation/Notifies/AnimNotifyState_Hitbox.h"

#include "Animation/AnimSequenceHelpers.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialRenderProxy.h"

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

		switch (HitboxParameters.Shape)
		{
		default:
			break;
		case EMeleeHitboxShape::Sphere:
			DrawSphereHitbox(PDI, CurrentPosition, HitboxParameters.Radius, HitboxDisplayColor);
			break;
		case EMeleeHitboxShape::Box:
			DrawBox(PDI, CurrentPosition, HitboxParameters.BoxExtents, HitboxParameters.Rotation, HitboxDisplayColor);
			break;
		case EMeleeHitboxShape::Capsule:
			DrawCapsuleHitbox(PDI, CurrentPosition, HitboxParameters.Radius, HitboxParameters.HalfHeight, HitboxParameters.Rotation, HitboxDisplayColor);
			break;
		}
		DrawCoordinateSystem(PDI, CurrentPosition, HitboxParameters.Rotation, 100.0f, ESceneDepthPriorityGroup::SDPG_Foreground);
		//PDI->SetHitProxy(nullptr);
	}
}

void UAnimNotifyState_Hitbox::DrawCanvasInEditor(FCanvas& Canvas, FSceneView& View, USkeletalMeshComponent* MeshComp,
	const UAnimSequenceBase* Animation, const FAnimNotifyEvent& NotifyEvent) const
{
	Super::DrawCanvasInEditor(Canvas, View, MeshComp, Animation, NotifyEvent);
}

void UAnimNotifyState_Hitbox::DrawBox(FPrimitiveDrawInterface* PDI, FVector CurrentPosition, FVector Box, FRotator Rotation, FLinearColor Color)
{
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector( Box.X,  Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector( Box.X, -Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector( Box.X, -Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector(-Box.X, -Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector(-Box.X, -Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector(-Box.X,  Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector(-Box.X,  Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector( Box.X,  Box.Y, Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector( Box.X,  Box.Y, -Box.Z)), CurrentPosition + Rotation.RotateVector(FVector( Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector( Box.X, -Box.Y, -Box.Z)), CurrentPosition + Rotation.RotateVector(FVector(-Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector(-Box.X, -Box.Y, -Box.Z)), CurrentPosition + Rotation.RotateVector(FVector(-Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector(-Box.X,  Box.Y, -Box.Z)), CurrentPosition + Rotation.RotateVector(FVector( Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector( Box.X,  Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector( Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector( Box.X, -Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector( Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector(-Box.X, -Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector(-Box.X, -Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
		PDI->DrawLine(CurrentPosition + Rotation.RotateVector(FVector(-Box.X,  Box.Y,  Box.Z)), CurrentPosition + Rotation.RotateVector(FVector(-Box.X,  Box.Y, -Box.Z)), Color, ESceneDepthPriorityGroup::SDPG_World);
}

void UAnimNotifyState_Hitbox::DrawSphereHitbox(class FPrimitiveDrawInterface* PDI, FVector CurrentPosition, float Radius, FLinearColor Color)
{
	uint8 Segments = 16;

	const float AngleInc = 2.f * UE_PI / Segments;
	int32 NumSegmentsY = 16;
	float Latitude = AngleInc;
	float SinY1 = 0.0f, CosY1 = 1.0f;

	while (NumSegmentsY--)
	{
		const float SinY2 = FMath::Sin(Latitude);
		const float CosY2 = FMath::Cos(Latitude);

		FVector Vertex1 = FVector(SinY1, 0.0f, CosY1) * Radius + CurrentPosition;
		FVector Vertex3 = FVector(SinY2, 0.0f, CosY2) * Radius + CurrentPosition;
		float Longitude = AngleInc;

		int32 NumSegmentsX = Segments;
		while (NumSegmentsX--)
		{
			const float SinX = FMath::Sin(Longitude);
			const float CosX = FMath::Cos(Longitude);

			const FVector Vertex2 = FVector((CosX * SinY1), (SinX * SinY1), CosY1) * Radius + CurrentPosition;
			const FVector Vertex4 = FVector((CosX * SinY2), (SinX * SinY2), CosY2) * Radius + CurrentPosition;

			PDI->DrawLine(Vertex1, Vertex2, Color, ESceneDepthPriorityGroup::SDPG_World);
			PDI->DrawLine(Vertex1, Vertex3, Color, ESceneDepthPriorityGroup::SDPG_World);

			Vertex1 = Vertex2;
			Vertex3 = Vertex4;
			Longitude += AngleInc;
		}
		SinY1 = SinY2;
		CosY1 = CosY2;
		Latitude += AngleInc;
	}
}

void UAnimNotifyState_Hitbox::DrawCapsuleHitbox(class FPrimitiveDrawInterface* PDI, FVector CurrentPosition,
	float Radius, float HalfHeight, FRotator Rotation, FLinearColor Color)
{
	constexpr int32 DrawCollisionSides = 16;
	const FVector Origin = CurrentPosition;
	const FMatrix Axes = FQuatRotationTranslationMatrix(Rotation.Quaternion(), FVector::ZeroVector);
	const FVector XAxis = Axes.GetScaledAxis( EAxis::X );
	const FVector YAxis = Axes.GetScaledAxis( EAxis::Y );
	const FVector ZAxis = Axes.GetScaledAxis( EAxis::Z ); 

	// Draw top and bottom circles
	const float HalfAxis = FMath::Max<float>(HalfHeight - Radius, 1.f);
	const FVector TopEnd = Origin + HalfAxis*ZAxis;
	const FVector BottomEnd = Origin - HalfAxis*ZAxis;

	DrawCircle(PDI, TopEnd, XAxis, YAxis, Color, Radius, DrawCollisionSides);
	DrawCircle(PDI, BottomEnd, XAxis, YAxis, Color, Radius, DrawCollisionSides);

	// Draw domed caps
	DrawHalfCircle(PDI, TopEnd, YAxis, ZAxis, Color, Radius, DrawCollisionSides);
	DrawHalfCircle(PDI, TopEnd, XAxis, ZAxis, Color, Radius, DrawCollisionSides);

	const FVector NegZAxis = -ZAxis;

	DrawHalfCircle(PDI, BottomEnd, YAxis, NegZAxis, Color, Radius, DrawCollisionSides);
	DrawHalfCircle(PDI, BottomEnd, XAxis, NegZAxis, Color, Radius, DrawCollisionSides);

	// Draw connected lines
	PDI->DrawLine(TopEnd + Radius*XAxis, BottomEnd + Radius*XAxis, Color, ESceneDepthPriorityGroup::SDPG_World);
	PDI->DrawLine(TopEnd - Radius*XAxis, BottomEnd - Radius*XAxis, Color, ESceneDepthPriorityGroup::SDPG_World);
	PDI->DrawLine(TopEnd + Radius*YAxis, BottomEnd + Radius*YAxis, Color, ESceneDepthPriorityGroup::SDPG_World);
	PDI->DrawLine(TopEnd - Radius*YAxis, BottomEnd - Radius*YAxis, Color, ESceneDepthPriorityGroup::SDPG_World);
	
}

void UAnimNotifyState_Hitbox::DrawHalfCircle(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X, const FVector& Y,
	const FLinearColor& Color, const float Radius, int32 NumSides)
{
	NumSides = FMath::Max(NumSides, 2);
	const float AngleDelta = 2.0f * UE_PI / NumSides;
	FVector	LastVertex = Base + X * Radius;

	for( int32 SideIndex = 0; SideIndex < (NumSides/2); SideIndex++)
	{
		FVector	Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		PDI->DrawLine(LastVertex, Vertex, Color, ESceneDepthPriorityGroup::SDPG_World);
		LastVertex = Vertex;
	}
}

void UAnimNotifyState_Hitbox::DrawCircle(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X, const FVector& Y,
	const FLinearColor& Color, const float Radius, int32 NumSides)
{
	NumSides = FMath::Max(NumSides, 2);
	const float	AngleDelta = 2.0f * UE_PI / NumSides;
	FVector	LastVertex = Base + X * Radius;

	for (int32 SideIndex = 0; SideIndex < NumSides; SideIndex++)
	{
		const FVector Vertex = Base + (X * FMath::Cos(AngleDelta * (SideIndex + 1)) + Y * FMath::Sin(AngleDelta * (SideIndex + 1))) * Radius;
		PDI->DrawLine(LastVertex, Vertex, Color, ESceneDepthPriorityGroup::SDPG_World);
		LastVertex = Vertex;
	}
}


#endif
