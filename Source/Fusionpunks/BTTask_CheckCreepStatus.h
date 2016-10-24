// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckCreepStatus.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_CheckCreepStatus : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class ACreep* creepTarget;
	class ACreepCamp* campTarget;
	
	
};
