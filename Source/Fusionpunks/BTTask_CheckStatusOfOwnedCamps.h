// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckStatusOfOwnedCamps.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_CheckStatusOfOwnedCamps : public UBTTaskNode
{
	GENERATED_BODY()
	
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
		
	class AHeroBase *hero, *enemyHero;
	class AHeroAIController* heroAI;
	class ACreepCamp *campBeingAttacked, *otherCampObjective;

	UPROPERTY(EditAnywhere)
		int32 numCreepsAlwaysDefend =5;
	UPROPERTY(EditAnywhere)
		float ignoreDefendPercentage = 0.6f;
	
	UPROPERTY(EditAnywhere)
		int32 creepDifferenceAllowed = 3;
	UPROPERTY(EditAnywhere)
		float healthPercentRequired = 0.15f;
	UPROPERTY(EditAnywhere)
		float healthPercentDifferenceAllowed = 0.5f;
	UPROPERTY(EditAnywhere)
		int32  levelDifferenceAllowed = 10;
	
	
};
