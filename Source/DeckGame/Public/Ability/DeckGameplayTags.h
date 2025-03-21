// Copyright (c) 2025 Stephen Melnick

#pragma once

#include "NativeGameplayTags.h"

namespace DeckGameplayTags
{
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Scroll);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Attack);

	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slot_Select);
	
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slot_0);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slot_1);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slot_2);
	
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slot_0_Select);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slot_1_Select);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Slot_2_Select);

	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEffect_Stun);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEffect_Frozen);
	
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Respawn);
	
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_AbilityTrigger_SelectSlot);

	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Duration);

	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTree_Perception_Sensed)
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StateTree_Perception_Forgotten)
	
#if !UE_BUILD_SHIPPING
	DECKGAME_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_Immortal);
#endif // !UE_BUILD_SHIPPING

}