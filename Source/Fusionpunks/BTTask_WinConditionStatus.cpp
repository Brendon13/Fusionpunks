// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_WinConditionStatus.h"




EBTNodeResult::Type UBTTask_WinConditionStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (hero != nullptr)
	{
		if (hero->GetArmySize() >= creepWinAmount)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("GoingForWin", true);
			return EBTNodeResult::Succeeded;
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("GoingForWin", false);
		return EBTNodeResult::Failed;



	}
	return EBTNodeResult::Failed;
	



}