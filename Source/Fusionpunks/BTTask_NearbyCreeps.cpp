// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creep.h"
#include "BTTask_NearbyCreeps.h"


EBTNodeResult::Type UBTTask_NearbyCreeps::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (hero != nullptr)
	{
		if (hero->CheckForNearbyEnemyCreeps())
		{			
			TArray<ACreep*> enemyCreeps = hero->GetNearbyEnemyCreeps();
			if (enemyCreeps.Num() > 0)
			{		
				ACreep* target = enemyCreeps[0];
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("AttackTarget", target);
				return EBTNodeResult::Succeeded;
			}

			return EBTNodeResult::Failed;
					
		}

		else
		{
			return EBTNodeResult::Failed;
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero!"));
		return EBTNodeResult::Failed;
	}

}


