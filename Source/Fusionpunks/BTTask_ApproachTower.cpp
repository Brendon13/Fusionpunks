// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "BTTask_ApproachTower.h"

EBTNodeResult::Type UBTTask_ApproachTower::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI != nullptr)
	{

		attackTarget = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

		if (attackTarget != nullptr)
		{
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
		}

		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}


void UBTTask_ApproachTower::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	
	if (hero->GetDistanceTo(attackTarget) > 500.0f)
	{
		OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 300.0f, false, true, false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY TOWER"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}