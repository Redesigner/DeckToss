// Copyright (c) 2025 Stephen Melnick

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

#include "DeckLayout.generated.h"

class UDeckLayoutMap;
class UCommonActivatableWidget;
class UCommonActivatableWidgetContainerBase;
class UNotificationDataAsset;
class UNotificationWidget;

UCLASS()
class DECKUI_API UDeckLayout : public UCommonUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDeckLayoutMap> LayoutMap;

	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> LayerMap;

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterLayer(UCommonActivatableWidgetContainerBase* Layer, UPARAM(meta = (Categories = "UI.SlotTag")) FGameplayTag SlotTag);

private:
	bool Initialize() override;

	UCommonActivatableWidget* PushWidgetToLayer(TSoftClassPtr<UCommonActivatableWidget> WidgetClass, FGameplayTag LayerTag);
};
