// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "AIController.h"
#include "CreepCamp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creep.h"
#include "BTTask_DecideHowToApproachHero.h"


EBTNodeResult::Type UBTTask_DecideHowToApproachHero::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (hero != nullptr)
	{
		heroStats = hero->GetHeroStats();
		attackTarget = Cast<AHeroBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		campTarget = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));

		if (attackTarget != nullptr && campTarget!= nullptr)
		{

			if (hero->GetCurrentHealth() <= healthPercentRequired)
			{
				approachStatus = EApproachStatus::AS_Escaping;
			
			}


			else if (hero->IsCapturing() || hero->GetDistanceTo(campTarget) <= 700)
			{	
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("HeroAttackSituationTarget", campTarget);
				approachStatus = EApproachStatus::AS_DefendingCamp;
				
			}


			else if (attackTarget->GetArmySize() - hero->GetArmySize() <= creepDifferenceAllowed)
			{
				approachStatus = EApproachStatus::AS_AgressiveChase;
				
			}

			else
			{
				approachStatus = EApproachStatus::AS_Escaping;
				
			}
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
			
		}
		UE_LOG(LogTemp, Error, TEXT("Targets are null"));
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero"));
		return EBTNodeResult::Failed;
	}


}

void UBTTask_DecideHowToApproachHero::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (approachStatus == EApproachStatus::AS_DefendingCamp) 
	{
		if (attackTarget->IsCapturing())
		{
			OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 50, false, true, false);
		}
		else
		{
			if (hero->GetDistanceTo(campTarget) >= 600)
			{
				OwnerComp.GetAIOwner()->MoveToActor(campTarget, 500, true, true, false);
			}
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			
		}
	
	
	}

	else if (approachStatus == EApproachStatus::AS_AgressiveChase)
	{

	}
}