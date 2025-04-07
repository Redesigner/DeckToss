// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "AI/EQS/EnvQueryGenerator_Line.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

void UEnvQueryGenerator_Line::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* BindOwner = QueryInstance.Owner.Get();
	Mirror.BindData(BindOwner, QueryInstance.QueryID);
	NumPoints.BindData(BindOwner, QueryInstance.QueryID);
	SpaceBetween.BindData(BindOwner, QueryInstance.QueryID);

	TArray<FVector> ContextLocations;
	QueryInstance.PrepareContext(GenerateAround, ContextLocations);

	const bool bMirrorValue = Mirror.GetValue();
	const int NumPointsValue = NumPoints.GetValue();
	const float SpaceBetweenValue = SpaceBetween.GetValue();
	
	TArray<FNavLocation> GridPoints;
	GridPoints.Reserve((bMirrorValue ? NumPointsValue * 2 : NumPointsValue) * ContextLocations.Num());

	for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
	{
		const FVector StartingLocation = ContextLocations[ContextIndex];
		for (int32 i = 0; i < NumPointsValue; ++i)
		{
			GridPoints.Add(FNavLocation(StartingLocation + FVector(0.0f, (i + 1) * SpaceBetweenValue, 0.0f)));
		}
	}

	if (bMirrorValue)
	{
		for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
		{
			const FVector StartingLocation = ContextLocations[ContextIndex];
			for (int32 i = 0; i < NumPointsValue; ++i)
			{
				GridPoints.Add(FNavLocation(StartingLocation + FVector(0.0f, (i + 1) * -SpaceBetweenValue, 0.0f)));
			}
		}
	}

	ProjectAndFilterNavPoints(GridPoints, QueryInstance);
	StoreNavPoints(GridPoints, QueryInstance);
}

FText UEnvQueryGenerator_Line::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("LineDescriptionGenerateAroundContext", "{0}: generate around {1}"),
		Super::GetDescriptionTitle(), UEnvQueryTypes::DescribeContext(GenerateAround));
}

FText UEnvQueryGenerator_Line::GetDescriptionDetails() const
{
	FText Desc = FText::Format(LOCTEXT("LineDescription", "points: {0}, space between: {1}, mirrored: {2}"),
		FText::FromString(NumPoints.ToString()), FText::FromString(SpaceBetween.ToString()), FText::FromString(Mirror.ToString()));

	FText ProjDesc = ProjectionData.ToText(FEnvTraceData::Brief);
	if (!ProjDesc.IsEmpty())
	{
		FFormatNamedArguments ProjArgs;
		ProjArgs.Add(TEXT("Description"), Desc);
		ProjArgs.Add(TEXT("ProjectionDescription"), ProjDesc);
		Desc = FText::Format(LOCTEXT("LineDescriptionWithProjection", "{Description}, {ProjectionDescription}"), ProjArgs);
	}

	return Desc;}

#undef LOCTEXT_NAMESPACE