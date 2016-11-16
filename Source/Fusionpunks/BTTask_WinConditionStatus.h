// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WinConditionStatus.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_WinConditionStatus : public UBTTaskNode
{
	GENERATED_BODY()

protected:
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
		class AHeroBase* hero;

		UPROPERTY(EditAnywhere)
			int32 creepWinAmount = 5;
	
};
