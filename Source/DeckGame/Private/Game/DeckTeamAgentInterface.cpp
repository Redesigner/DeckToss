// Copyright (c) 2024 Stephen Melnick


#include "Game/DeckTeamAgentInterface.h"

void IDeckTeamAgentInterface::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	SetDeckTeam(GetTeamIdAsEnum(TeamID));
}

FGenericTeamId IDeckTeamAgentInterface::GetGenericTeamId() const
{
	return GetTeamIdAsEnum(GetDeckTeam());
}

void IDeckTeamAgentInterface::ConditionalBroadcastTeamChanged(TScriptInterface<IDeckTeamAgentInterface> This, EDeckTeam OldTeamID, EDeckTeam NewTeamID)
{
	if (OldTeamID != NewTeamID)
	{
		UObject* ThisObj = This.GetObject();
		if (FOnTeamChangedDelegate* OnTeamChangedDelegate = This.GetInterface()->GetOnTeamChangedDelegate())
		{
			OnTeamChangedDelegate->Broadcast(ThisObj, OldTeamID, NewTeamID);
		}
	}
}

EDeckTeam IDeckTeamAgentInterface::GetTeamIdAsEnum(FGenericTeamId GenericTeamID)
{
	if (GenericTeamID == FGenericTeamId::NoTeam)
	{
		return EDeckTeam::Unaffiliated;
	}

	if (GenericTeamID.GetId() >= EDeckTeam::Unaffiliated)
	{
		return EDeckTeam::Unaffiliated;
	}

	return EDeckTeam(GenericTeamID.GetId());
}