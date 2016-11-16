// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creep.h"
#include "BTTask_FindNearbyEnemy.h"


EBTNodeResult::Type UBTTask_FindNearbyEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (hero != nullptr)
	{
		if (enemyType == ETargetEnemyType::TE_Creeps)
		{
			if (hero->CheckForNearbyEnemyCreeps())
			{
				TArray<ACreep*> enemyCreeps = hero->GetNearbyEnemyCreeps();
				
				if (enemyCreeps.Num() > 0)
				{
					//UE_LOG(LogTemp, Error, TEXT("Found A Creep Target"));
					ACreep* target = enemyCreeps[0];
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", target);
					return EBTNodeResult::Succeeded;
				}
			}

			//UE_LOG(LogTemp, Error, TEXT("No Creeps Nearby"));
			return EBTNodeResult::Failed;
		}

		else if (enemyType == ETargetEnemyType::TE_Hero)
		{
			if (hero->CheckForNearbyEnemyHero())
			{
				UE_LOG(LogTemp, Error, TEXT("Found Enemy Hero Target"));
				AHeroBase* enemyHero = hero->GetNearbyEnemyHero();
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", enemyHero);
				return EBTNodeResult::Succeeded;
			}
			//UE_LOG(LogTemp, Error, TEXT("No Hero Nearby"));
			return EBTNodeResult::Failed;

		}

		else if (enemyType == ETargetEnemyType::TE_Tower)
		{
			if (hero->CheckForNearbyEnemyTowers())
			{
				UE_LOG(LogTemp, Error, TEXT("Found Enemy Tower Target"));
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", Cast<AActor>(hero->GetNearbyEnemyTower()));
				return EBTNodeResult::Succeeded;
			}
			return EBTNodeResult::Failed;
		}



		UE_LOG(LogTemp, Error, TEXT("werid shit happenin"));
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero! Find Nearby Enemy Task"));
		return EBTNodeResult::Failed;
	}

	
}


