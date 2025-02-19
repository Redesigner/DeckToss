// Copyright (c) 2024 Stephen Melnick


#include "Ability/DeckGameplayTags.h"

namespace DeckGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "Look input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Attack, "InputTag.Attack", "Default attack input.");

	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Death, "GameplayEvent.Death");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Respawn, "GameplayEvent.Respawn");

	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Damage, "SetByCaller.Damage");
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Duration, "SetByCaller.Duration");

#if !UE_BUILD_SHIPPING
	UE_DEFINE_GAMEPLAY_TAG(Cheat_Immortal, "Cheat.Immortal");
#endif // !UE_BUILD_SHIPPING
}