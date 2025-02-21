// Copyright (c) 2025 Stephen Melnick


#include "Layout/DeckLayoutMap.h"

bool UDeckLayoutMap::FindWidgetLayoutForMessage(FGameplayTag MessageTag, FDeckLayoutMapEntry& WidgetLayoutOut) const
{
	for (const FDeckLayoutMapEntry& Entry : LayoutEntries)
	{
		if (Entry.MessageTag == MessageTag)
		{
			WidgetLayoutOut = Entry;
			return true;
		}
	}

	return false;
}
