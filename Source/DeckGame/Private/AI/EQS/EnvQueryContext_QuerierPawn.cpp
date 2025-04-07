// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/EQS/EnvQueryContext_QuerierPawn.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_QuerierPawn::ProvideContext(FEnvQueryInstance& QueryInstance,
                                                  FEnvQueryContextData& ContextData) const
{
	if (AController* AIController = Cast<AController>(QueryInstance.Owner.Get()))
	{
		if (APawn* Pawn = AIController->GetPawn())
		{
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Pawn);
		}
	}
	
}
