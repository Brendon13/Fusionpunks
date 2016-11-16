// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ApproachTower.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_ApproachTower : public UBTTaskNode
{
	GENERATED_BODY()
		
protected:
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
		virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
		
		class AHeroBase* hero;
		class AHeroAIController* heroAI;
		AActor* attackTarget;
		//EPathFollowingRequestResult epath;
};
