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
}

void ADeckAIController::BeginPlay()
{
	Super::BeginPlay();
	
	StateTreeComponent->StartLogic();
	AIPerception->OnTargetPerceptionInfoUpdated.AddUniqueDynamic(this, &ThisClass::TargetPerceptionInfoUpdated);
	AIPerception->OnTargetPerceptionForgotten.AddUniqueDynamic(this, &ThisClass::TargetPerceptionForgotten);

	TargetLocationProxy = GetWorld()->SpawnActor(TargetProxyClass);
}

void ADeckAIController::TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (UpdateInfo.Stimulus.IsActive())
	{
		UE_LOGFMT(LogDeckGame, Display, "DeckAIController: Perception sensed '{PerceptionTarget}' with sense '{SenseName}'",
			UpdateInfo.Target.IsValid() ? UpdateInfo.Target->GetName() : "null",
			UpdateInfo.Stimulus.Type.Name);
		StateTreeComponent->SendStateTreeEvent(DeckGameplayTags::StateTree_Perception_Sensed, FConstStructView::Make(UpdateInfo.Stimulus));
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

	UE_LOGFMT(LogDeckGame, Display, "DeckAIController '{ControllerName}': Perception Forgotten", GetName());
	AIPerception->GetKnownPerceivedActors(nullptr, PerceivedActors);
	StateTreeComponent->SendStateTreeEvent(DeckGameplayTags::StateTree_Perception_Forgotten);
}

AActor* ADeckAIController::GetTargetLocationProxy() const
{
	return TargetLocationProxy.Get();
}