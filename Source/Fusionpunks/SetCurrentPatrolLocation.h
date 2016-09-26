// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "SetCurrentPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API USetCurrentPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override; 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
};
