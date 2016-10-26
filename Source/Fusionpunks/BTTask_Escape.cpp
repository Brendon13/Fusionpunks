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
		healingWell = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("HealingWell"));
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
		return EBTNodeResult::Failed;
}

void UBTTask_Escape::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!hero->InsideHealingWell())
	{
		OwnerComp.GetAIOwner()->MoveToActor(healingWell, 50, true, true, false);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("MADE IT TO HEALING WELL"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}





