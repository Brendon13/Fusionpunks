// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "BTTask_ApproachBase.h"

EBTNodeResult::Type UBTTask_ApproachBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI != nullptr)
	{

		enemyBase= Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnemyBase"));

		if (enemyBase != nullptr)
		{
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
		}

		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}


void UBTTask_ApproachBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (hero->CheckForNearbyInteractions())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("FoundNearbyEnemyCamp", hero->GetNearbyEnemyCamp() != nullptr);
		if (hero->GetNearbyEnemyCamp() != nullptr)
		{
			if (heroAI->SafetyCheck(hero->GetNearbyEnemyCamp()))
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("NearbyEnemyCamp", hero->GetNearbyEnemyCamp());
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}


	else
	{
		if (hero->GetDistanceTo(enemyBase) >= 300)
		{
			OwnerComp.GetAIOwner()->MoveToActor(enemyBase, 50, false, true, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY HERO"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}


