// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "UObject/Interface.h"
#include "DeckTeamAgentInterface.generated.h"

UENUM(BlueprintType)
enum EDeckTeam
{
	Friendly,
	Enemy,
	Unaffiliated
};

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UDeckTeamAgentInterface : public UGenericTeamAgentInterface
{
	GENERATED_BODY()
};

class DECKGAME_API IDeckTeamAgentInterface : public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	// Begin IGenericTeamAgentInterface
	void SetGenericTeamId(const FGenericTeamId& TeamID) override final;
	FGenericTeamId GetGenericTeamId() const override final;
	// End IGenericTeamAgentInterface

	static EDeckTeam GetTeamIdAsEnum(FGenericTeamId GenericTeamID);

public:
	virtual void SetDeckTeam(EDeckTeam InTeam) = 0;
	virtual EDeckTeam GetDeckTeam() const = 0;
};
