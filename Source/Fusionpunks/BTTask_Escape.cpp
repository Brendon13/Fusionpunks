// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AIController.h"
#include "HeroBase.h"
#include "HeroStats.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_Escape.h"

EBTNodeResult::Type UBTTask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (hero != nullptr)
	{
		heroStats = hero->GetHeroStats();

		
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
		return EBTNodeResult::Failed;
}





