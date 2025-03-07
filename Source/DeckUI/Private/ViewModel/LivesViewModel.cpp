// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales


#include "ViewModel/LivesViewModel.h"

#include "Character/DeckPlayerState.h"

void ULivesViewModel::BindPlayerState(ADeckPlayerState* DeckPlayerState)
{
	DeckPlayerState->OnLivesChanged.AddUObject(this, &ThisClass::LivesCountChanged);
	UE_MVVM_SET_PROPERTY_VALUE(LivesCount, DeckPlayerState->GetLivesCount());
}

void ULivesViewModel::LivesCountChanged(uint8 NewCount)
{
	UE_MVVM_SET_PROPERTY_VALUE(LivesCount, NewCount);
}
