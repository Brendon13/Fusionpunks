// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveInRangeOfEnemy.generated.h"

/**
 * 
 */


UCLASS()
class FUSIONPUNKS_API UBTTask_MoveInRangeOfEnemy : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	class AHeroBase* hero;
	class HeroStats* heroStats;
	class AHeroAIController* heroAI;
	AActor* target;

	
	
};
