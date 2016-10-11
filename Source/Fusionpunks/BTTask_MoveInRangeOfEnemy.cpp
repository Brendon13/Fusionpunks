// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AIController.h"
#include "HeroBase.h"
#include "Creep.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_MoveInRangeOfEnemy.h"


EBTNodeResult::Type UBTTask_MoveInRangeOfEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (hero != nullptr)
	{
		heroStats = hero->GetHeroStats();
		target = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		if (target != nullptr)
		{
			if (hero->GetDistanceTo(target) >= 5)
			{
				bNotifyTick = true;
				return EBTNodeResult::InProgress;
			}

			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero"));
		return EBTNodeResult::Failed;
	}

	

}
void UBTTask_MoveInRangeOfEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);




}

