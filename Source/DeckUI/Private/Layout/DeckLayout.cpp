// Copyright (c) 2024 Stephen Melnick


#include "Layout/DeckLayout.h"

#include "CommonActivatableWidget.h"
#include "Logging/StructuredLog.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

#include "DeckUI.h"
#include "Layout/DeckLayoutMap.h"


UE_DEFINE_GAMEPLAY_TAG(UI_SlotTag_Notification, "UI.SlotTag.Notification");

void UDeckLayout::RegisterLayer(UCommonActivatableWidgetContainerBase* Layer, FGameplayTag SlotTag)
{
	if (LayerMap.Contains(SlotTag))
	{
		return;
	}

	LayerMap.Add(SlotTag, Layer);
}

bool UDeckLayout::Initialize()
{
	if (!IsDesignTime())
	{
	}

	return Super::Initialize();
}

UCommonActivatableWidget* UDeckLayout::PushWidgetToLayer(TSoftClassPtr<UCommonActivatableWidget> WidgetClass, FGameplayTag LayerTag)
{
	if (WidgetClass.IsNull())
	{
		return nullptr;
	}

	if (WidgetClass.IsPending())
	{
		UE_LOGFMT(LogDeckUI, Warning, "DeckLayout '{LayoutName}' creating widget of type '{WidgetClassName}'. The widget class was not loaded, loading synchronously.", GetName(), WidgetClass.ToString());
		WidgetClass.LoadSynchronous();
	}

	TObjectPtr<UCommonActivatableWidgetContainerBase> Layer = LayerMap.FindRef(LayerTag);
	if (!Layer)
	{
		UE_LOGFMT(LogDeckUI, Warning, "DeckLayout '{LayoutName}' failed to push widget '{WidgetClassName}' to layer. Could not find layer '{LayerName}'.", GetName(), WidgetClass.ToString(), LayerTag.ToString());
		return nullptr;
	}

	return Layer->AddWidget(WidgetClass.Get());
}
