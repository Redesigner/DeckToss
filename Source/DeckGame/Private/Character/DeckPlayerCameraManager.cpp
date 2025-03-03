// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Character/DeckPlayerCameraManager.h"

#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

TOptional<FVector> ADeckPlayerCameraManager::CalculateCameraPosition() const
{

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
	if (!GameState)
	{
		return NullOpt;
	}

	FVector Origin = FVector::ZeroVector;
	int PlayerCount = 0;

	bool bBoundsDefined = false;
	float LeftPosition = 0.0f;
	float RightPosition = 0.0f;
	float TopPosition = 0.0f;
	float BottomPosition = 0.0f;

	FMinimalViewInfo CameraView = GetCameraCacheView();
	FVector CameraRight = CameraView.Rotation.RotateVector(FVector::RightVector);
	FVector CameraUp = CameraView.Rotation.RotateVector(FVector::UpVector);
	FVector CameraForward = CameraView.Rotation.RotateVector(FVector::ForwardVector);

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		APawn* Player = PlayerState->GetPawn();
		if (!Player)
		{
			continue;
		}

		const FVector PlayerPosition = Player->GetActorLocation();
		if (!bBoundsDefined)
		{
			bBoundsDefined = true;
			LeftPosition = PlayerPosition.Dot(CameraRight);
			RightPosition = LeftPosition;
			TopPosition = PlayerPosition.Dot(CameraUp);
			BottomPosition = TopPosition;
		}
		else
		{
			float PlayerXPosition = PlayerPosition.Dot(CameraRight);
			float PlayerYPosition = PlayerPosition.Dot(CameraUp);
			if (PlayerXPosition < LeftPosition)
			{
				LeftPosition = PlayerXPosition;
			}
			else if (PlayerXPosition > RightPosition)
			{
				RightPosition = PlayerXPosition;
			}

			if (PlayerYPosition > TopPosition)
			{
				TopPosition = PlayerYPosition;
			}
			else if (PlayerYPosition < BottomPosition)
			{
				BottomPosition = PlayerYPosition;
			}

		}
		PlayerCount++;
		Origin += Player->GetActorLocation();
	}

	// This is possible if all players have died!
	if (PlayerCount == 0)
	{
		return NullOpt;
	}

	const float CameraFOVV = CameraView.FOV / CameraView.AspectRatio;
	const float ZoomDistanceY = ((TopPosition - BottomPosition) / 2.0f) * CameraPaddingYFactor * FMath::Tan(FMath::DegreesToRadians(CameraFOVV) / 2.0f);
	const float ZoomDistanceX = ((RightPosition - LeftPosition) / 2.0f) * CameraPaddingXFactor * FMath::Tan(FMath::DegreesToRadians(CameraView.FOV) / 2.0f);

	const float ZoomDistance = FMath::Clamp(FMath::Max(ZoomDistanceX, ZoomDistanceY), CameraMinZoomDistance, CameraMaxZoomDistance);

	return TOptional<FVector>(
		CameraForward * -ZoomDistance +
		CameraRight * (RightPosition + LeftPosition) / 2.0f +
		CameraUp * (TopPosition + BottomPosition) / 2.0f);
}

void ADeckPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	// Don't update camera if the relevant player is not using a viewport, since this isn't splitscreen
	if (!PCOwner->GetLocalPlayer() || !PCOwner->GetLocalPlayer()->ViewportClient)
	{
		return;
	}
	FMinimalViewInfo CameraView = GetCameraCacheView();
	TOptional<FVector> NewCameraPosition = CalculateCameraPosition();
	if (NewCameraPosition)
	{
		CameraView.Location = FMath::Lerp(PreviousCameraPosition, NewCameraPosition.GetValue(), FMath::Clamp(CameraTrackingLerpSpeed * DeltaTime, 0.0f, 1.0f));
		PreviousCameraPosition = CameraView.Location;
		SetCameraCachePOV(CameraView);
	}
}