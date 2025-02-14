// Copyright (c) 2024 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"

#include "DeckTeamAgentInterface.generated.h"

UENUM(BlueprintType)
enum EDeckTeam
{
	Allied,
	Enemy,
	Unaffiliated
};


template <typename InterfaceType> class TScriptInterface;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTeamChangedDelegate, UObject*, ObjectChangingTeam, EDeckTeam, OldTeamID, EDeckTeam, NewTeamID);

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class DECKGAME_API UDeckTeamAgentInterface : public UGenericTeamAgentInterface
{
	GENERATED_BODY()
};

// The Interface is not completed yet, it mostly shadows GenericTeamAgentInterface for now
class DECKGAME_API IDeckTeamAgentInterface : public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	// Begin IGenericTeamAgentInterface
	void SetGenericTeamId(const FGenericTeamId& TeamID) override final;
	FGenericTeamId GetGenericTeamId() const override final;
	// End IGenericTeamAgentInterface

	virtual FOnTeamChangedDelegate* GetOnTeamChangedDelegate() { return nullptr; }

	static void ConditionalBroadcastTeamChanged(TScriptInterface<IDeckTeamAgentInterface> This, EDeckTeam OldTeamID, EDeckTeam NewTeamID);

	static EDeckTeam GetTeamIdAsEnum(FGenericTeamId GenericTeamID);

public:
	virtual void OverrideTeam(EDeckTeam NewTeam) = 0;
	virtual void SetDeckTeam(EDeckTeam InTeam) = 0;
	virtual EDeckTeam GetDeckTeam() const = 0;
};