// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToCamp.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_MoveToCamp : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
		virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
