// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include"AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "DeckAIController.generated.h"

struct FActorPerceptionUpdateInfo;
class UStateTreeAIComponent;

UCLASS()
class DECKGAME_API ADeckAIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;
	
	ADeckAIController();

	void BeginPlay() override; 

	UFUNCTION()
	void TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION()
	void TargetPerceptionForgotten(AActor* Actor);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> TargetProxyClass;

	TWeakObjectPtr<AActor> TargetLocationProxy;
	
public:
	AActor* GetTargetLocationProxy() const;

};