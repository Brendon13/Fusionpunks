// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "HeroBase.h"
#include "HeroStats.h"
#include "AbilityBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_Escape.h"

EBTNodeResult::Type UBTTask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI != nullptr)
	{		
		healingWell = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("HealingWell"));
		bNotifyTick = true;
		sacrificeCreepAbility = hero->GetAbility(3);
		return EBTNodeResult::InProgress;
	}
		return EBTNodeResult::Failed;
}

void UBTTask_Escape::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (hero->GetPlayerHealthAsDecimal() <= 0.5f)
	{
		if (hero->GetArmySize() > 0 && sacrificeCreepAbility != nullptr && sacrificeCreepAbility->CanUse())
		{
			UE_LOG(LogTemp, Error, TEXT("AI Sacrificed Creep"));
			sacrificeCreepAbility->Use();
		}
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

	else
	{
		heroAI->ResetAITreeTaskStatus();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}





