// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include"AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "DeckAIController.generated.h"

struct FActorPerceptionUpdateInfo;
class UStateTreeAIComponent;

UENUM(BlueprintType)
enum class EEnemyAIMovementState : uint8
{
	Wandering,
	Investigating,
	Following
};

UCLASS()
class DECKGAME_API ADeckAIController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Behavior, Meta = (AllowPrivateAccess))
	TObjectPtr<UStateTreeAIComponent> StateTree;


public:
	// Properties must be public in order for StateTree to read it!
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Meta = (AllowPrivateAccess))
	EEnemyAIMovementState MovementState = EEnemyAIMovementState::Wandering;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnMovementStateChanged(EEnemyAIMovementState OldState, EEnemyAIMovementState NewState);

private:
	ADeckAIController();

	void SetAIMovementState(EEnemyAIMovementState State);

	void BeginPlay() override; 

	UFUNCTION()
	void TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION()
	void TargetPerceptionForgotten(AActor* Actor);
};
