// Copyright (c) 2024 Stephen Melnick


#include "Ability/DeckGameplayTags.h"

namespace DeckGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "Look input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Scroll, "InputTag.Scroll", "Scroll input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Attack, "InputTag.Attack", "Default attack input.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Slot_0, "InputTag.Slot.0", "Input Tag for first card slot.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Slot_1, "InputTag.Slot.1", "Input Tag for second card slot.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Slot_2, "InputTag.Slot.2", "Input Tag for third card slot.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffect_Stun, "GameplayEffect.Stun", "Character cannot move, but can still be moved and fall due to gravity");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEffect_Frozen, "GameplayEffect.Frozen", "Character cannot move, and is not affected by external forces.");
	
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Death, "GameplayEvent.Death");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Respawn, "GameplayEvent.Respawn");

	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Damage, "SetByCaller.Damage");
	UE_DEFINE_GAMEPLAY_TAG(SetByCaller_Duration, "SetByCaller.Duration");

	UE_DEFINE_GAMEPLAY_TAG(StateTree_Perception_Sensed, "StateTree.Perception.Sensed");
	UE_DEFINE_GAMEPLAY_TAG(StateTree_Perception_Forgotten, "StateTree.Perception.Forgotten");

#if !UE_BUILD_SHIPPING
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_Immortal, "Cheat.Immortal", "Character is immortal -- they cannot die, but will still take damage.");
#endif // !UE_BUILD_SHIPPING
}