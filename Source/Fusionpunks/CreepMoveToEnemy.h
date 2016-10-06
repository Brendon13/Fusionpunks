// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CreepMoveToEnemy.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UCreepMoveToEnemy : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
	
};
