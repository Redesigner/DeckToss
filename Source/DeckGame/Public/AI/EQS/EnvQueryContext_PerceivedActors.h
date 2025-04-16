// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_PerceivedActors.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API UEnvQueryContext_PerceivedActors : public UEnvQueryContext
{
	GENERATED_BODY()

	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
