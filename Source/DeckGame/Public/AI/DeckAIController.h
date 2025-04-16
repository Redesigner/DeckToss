// Copyright (c) 2025 Paul Alicia, Stephen Melnick, Ian Morales

#pragma once

#include "CoreMinimal.h"
#include"AIController.h"
#include "Character/DeckTeamAgentInterface.h"
#include "Components/StateTreeAIComponent.h"
#include "DeckAIController.generated.h"

struct FActorPerceptionUpdateInfo;
class UStateTreeAIComponent;

UCLASS()
class DECKGAME_API ADeckAIController : public AAIController,
	public IDeckTeamAgentInterface
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Components, Meta = (AllowPrivateAccess))
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Behavior, Meta = (AllowPrivateAccess))
	TEnumAsByte<EDeckTeam> TeamId = EDeckTeam::Unaffiliated;
	
	ADeckAIController();

	void BeginPlay() override;

	void Destroyed() override;

	UFUNCTION()
	void TargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION()
	void TargetPerceptionForgotten(AActor* Actor);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Meta = (AllowPrivateAccess))
	TSubclassOf<AActor> TargetProxyClass;

	TWeakObjectPtr<AActor> TargetLocationProxy;

	void SetDeckTeam(EDeckTeam InTeam) override { TeamId = InTeam; }
	EDeckTeam GetDeckTeam() const override { return TeamId; }
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
public:
	AActor* GetTargetLocationProxy() const;

};