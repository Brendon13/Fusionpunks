// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_CreepWait.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_CreepWait : public UBTTask_Wait
{
	GENERATED_BODY()
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	

	
	
};
