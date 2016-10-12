// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AIHeroEscapeCheck.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_AIHeroEscapeCheck : public UBTTaskNode
{
	GENERATED_BODY()

protected:

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
		float healthPercentage;
	class AHeroBase* hero;
	class HeroStats* heroStats;
	
	
};
