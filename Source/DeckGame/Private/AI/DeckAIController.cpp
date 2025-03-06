// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/DeckAIController.h"

#include "Logging/StructuredLog.h"
#include "Perception/AIPerceptionComponent.h"

#include "DeckGame.h"
#include "Ability/DeckGameplayTags.h"


ADeckAIController::ADeckAIController()
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTree"));
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	AIPerception->OnTargetPerceptionInfoUpdated.AddUniqueDynamic(this, &ThisClass::TargetPerceptionInfoUpdated);
}

void ADeckAIController::BeginPlay()
{
	Super::BeginPlay();
	
	StateTreeComponent->StartLogic();
}

void ADeckAIController::TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (UpdateInfo.Stimulus.IsActive())
	{
		UE_LOGFMT(LogDeckGame, Display, "DeckAIController: Perception sensed '{PerceptionTarget}' with sense '{SenseName}'",
			UpdateInfo.Target.IsValid() ? UpdateInfo.Target->GetName() : "null",
			UpdateInfo.Stimulus.Type.Name);
		StateTreeComponent->SendStateTreeEvent(DeckGameplayTags::StateTree_Perception);
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
}
