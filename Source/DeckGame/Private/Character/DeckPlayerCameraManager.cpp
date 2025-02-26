// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Character/DeckPlayerCameraManager.h"

#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ADeckPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
	if (!GameState)
	{
		return;
	}

	FVector Origin = FVector::ZeroVector;
	int PlayerCount = 0;

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		APawn* Player = PlayerState->GetPawn();
		if (!Player)
		{
			continue;
		}
		PlayerCount++;
		Origin += Player->GetActorLocation();
	}

	FMinimalViewInfo CameraView = GetCameraCachePOV();
	FVector ZoomVector = CameraView.Rotation.RotateVector(FVector::ForwardVector) * -500.0f;
	CameraView.Location = Origin / static_cast<float>(PlayerCount) + ZoomVector;
	SetCameraCachePOV(CameraView);
}