// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/EQS/EnvQueryContext_LastPerceivedActor.h"

#include "AIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Perception/AIPerceptionComponent.h"

void UEnvQueryContext_LastPerceivedActor::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                         FEnvQueryContextData& ContextData) const
{
	AAIController* Controller = Cast<AAIController>(QueryInstance.Owner.Get());
	if (!Controller)
	{
		return;
	}

	if (!Controller->GetAIPerceptionComponent())
	{
		return;
	}

	TArray<AActor*> PerceivedActors;
	Controller->GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);
	if (PerceivedActors.IsEmpty())
	{
		return;
	}
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, PerceivedActors[0]);
}
