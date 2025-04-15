// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Character/DeckCharacter.h"

#include "GameFramework/PawnMovementComponent.h"

void ADeckCharacter::TeleportToStart()
{
	GetMovementComponent()->Velocity = FVector();
	TeleportTo(LastSafeLocation, GetActorRotation());
}

void ADeckCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	if (GetActorLocation().Z <= -1000.0f)
	{
		//TeleportToLastSafeLocation();
		TeleportToStart();
	}

	if (GetMesh())
	{
		if (GetActorForwardVector().Y > 0.0f)
		{
			GetMesh()->SetWorldRotation(FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 0.0f)));
		}
		else
		{
			GetMesh()->SetWorldRotation(FRotator::MakeFromEuler(FVector(0.0f, 0.0f, 180.0f)));
		}
	}
}

void ADeckCharacter::BeginPlay()
{
	Super::BeginPlay();

	LastSafeLocation = GetActorLocation();
}
