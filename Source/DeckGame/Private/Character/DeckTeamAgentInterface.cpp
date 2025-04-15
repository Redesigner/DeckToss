// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "Character/DeckTeamAgentInterface.h"


void IDeckTeamAgentInterface::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	SetDeckTeam(GetTeamIdAsEnum(TeamID));
}

FGenericTeamId IDeckTeamAgentInterface::GetGenericTeamId() const
{
	return GetTeamIdAsEnum(GetDeckTeam());
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
