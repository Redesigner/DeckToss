// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/DeckAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "StateTree.h"


ADeckAIController::ADeckAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	StateTree = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTree"));
}

void ADeckAIController::SetAIMovementState(EEnemyAIMovementState State)
{
	if (MovementState == State)
	{
		return;
	}

	EEnemyAIMovementState OldState = MovementState;
	MovementState = State;
	//Blackboard->SetValueAsEnum("MovementState", static_cast<uint8>(State));

	OnMovementStateChanged(OldState, State);
}

void ADeckAIController::BeginPlay()
{
	Super::BeginPlay();

	if (StateTree)
	{
		StateTree->StartLogic();
	}
}

void ADeckAIController::TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (UpdateInfo.Stimulus.IsActive())
	{
		SetAIMovementState(EEnemyAIMovementState::Following);
	}
}

void ADeckAIController::TargetPerceptionForgotten(AActor* Actor)
{
	if (!AIPerception)
	{
		return;
	}

	TArray<AActor*> PerceivedActors;
	AIPerception->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);
	// We're still perceiving some actors, so don't do anything
	if (!PerceivedActors.IsEmpty())
	{
		return;
	}

	AIPerception->GetKnownPerceivedActors(nullptr, PerceivedActors);
	if (PerceivedActors.IsEmpty())
	{
		// If we forget a perception, and don't remember any actors, switch to wandering
		SetAIMovementState(EEnemyAIMovementState::Wandering);
	}
}
