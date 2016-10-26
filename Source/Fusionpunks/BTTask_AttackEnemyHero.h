// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackEnemyHero.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_AttackEnemyHero : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	//virtual void OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult) override;
	class AHeroBase* hero;
	class HeroStats* heroStats;
	bool hasTarget = false;
	bool isAttacking = false;
	class AHeroBase* target;
	FTimerHandle attackTimerHandle;
	UPROPERTY(EditAnywhere)
		float attackSpeed = 0.35f;
	UPROPERTY(EditAnywhere)
		float healthPercentageAbort= 0.15f;
	UFUNCTION()
		void AttackOnTimer();
	
	
	
};
