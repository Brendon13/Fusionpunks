// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AIController.h"
#include "HeroBase.h"
#include "BTTask_CheckForEnemies.h"

EBTNodeResult::Type UBTTask_CheckForEnemies::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (hero != nullptr)
	{
	
		if (!hero->CheckForNearbyEnemyCreeps() && !hero->CheckForNearbyEnemyHero())
		{
			return EBTNodeResult::Succeeded;
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	else 
	{
		return EBTNodeResult::Failed;
	}

}


