// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "EnvQueryGenerator_Line.generated.h"

/**
 * 
 */
UCLASS()
class DECKGAME_API UEnvQueryGenerator_Line : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()

	/** generation density */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderFloatValue SpaceBetween;

	UPROPERTY(EditDefaultsOnly, Category=Generator, Meta = (DisplayName = "Number of Points"))
	FAIDataProviderIntValue NumPoints;
	
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	FAIDataProviderBoolValue Mirror;

	/** context */
	UPROPERTY(EditDefaultsOnly, Category=Generator)
	TSubclassOf<UEnvQueryContext> GenerateAround;
	
	void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	FText GetDescriptionTitle() const override;
	FText GetDescriptionDetails() const override;
};
