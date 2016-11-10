// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DetermineCampSafety.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_DetermineCampSafety : public UBTTaskNode
{
	GENERATED_BODY()
	
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
		float healthPercentage;

	UPROPERTY(EditAnywhere)
		int32 allowedCreepDifference;

	class AHeroBase* enemyHero;

};
