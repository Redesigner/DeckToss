// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_QuerierPawn.generated.h"

/**
 * Grab the pawn of the querier, if the querier is a Controller
 */
UCLASS()
class DECKGAME_API UEnvQueryContext_QuerierPawn : public UEnvQueryContext
{
	GENERATED_BODY()

	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
