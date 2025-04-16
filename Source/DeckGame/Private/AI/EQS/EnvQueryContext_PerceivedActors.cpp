// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/EQS/EnvQueryContext_PerceivedActors.h"

#include "AIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Perception/AIPerceptionComponent.h"

void UEnvQueryContext_PerceivedActors::ProvideContext(FEnvQueryInstance& QueryInstance,
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
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, PerceivedActors);
}
