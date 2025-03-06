// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "Misc/Optional.h"

#include "DeckPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API ADeckPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Tracking, Meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float CameraMinZoomDistance = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Tracking, Meta = (AllowPrivateAccess, ClampMin = 0.0f))
	float CameraMaxZoomDistance = 1000.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Tracking, Meta = (AllowPrivateAccess, ClampMin = 1.0f))
	float CameraPaddingXFactor = 1.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Tracking, Meta = (AllowPrivateAccess, ClampMin = 1.0f))
	float CameraPaddingYFactor = 4.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Tracking, Meta = (AllowPrivateAccess, ClampMin = 0.0f, ClampMax = 5.0f))
	float CameraTrackingLerpSpeed = 2.0f;

	TOptional<FVector> CalculateCameraPosition() const;
	
	void UpdateCamera(float DeltaTime) override;

	FVector PreviousCameraPosition;

	bool bCameraInitialized = false;
};
