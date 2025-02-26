// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DeckPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API ADeckPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	void UpdateCamera(float DeltaTime) override;
};
