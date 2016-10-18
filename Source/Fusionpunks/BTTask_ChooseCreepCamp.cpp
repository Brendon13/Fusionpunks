// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "CreepCamp.h"
#include "BTTask_ChooseCreepCamp.h"

EBTNodeResult::Type UBTTask_ChooseCreepCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
		return EBTNodeResult::Succeeded;

	AHeroAIController* heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	
	
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("CapturedCamp"))
	{	
		//CAMP HAS ALREADY BEEN CAPTURED SO CHOOSE NEW CAMP

		if (heroAI != nullptr)
		{
			TArray<ACreepCamp*> creepCamps = heroAI->GetCreepCampList();
			ACreepCamp* targetCamp = nullptr;

			AActor* hero = heroAI->GetPawn();
			ECampType enemyCampType;

			if (hero->ActorHasTag("Cyber"))
				enemyCampType = ECampType::CT_Diesel;

			else
				enemyCampType = ECampType::CT_Cyber;

			if (creepCamps.Num() > 0)
			{
				for (int32 i = 0; i < creepCamps.Num(); i++)
				{
					if ((creepCamps[i]->GetCampType() == ECampType::CT_Neutral || creepCamps[i]->GetCampType() == enemyCampType)
						&& creepCamps[i]->GetCampSafety())
					{
						targetCamp = creepCamps[i];
						break;
					}
				}

				if (targetCamp != nullptr)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("CampTarget", targetCamp);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", false);
				
					return EBTNodeResult::Succeeded;
				}

				// TARGET CAMP IS NULL 
				return EBTNodeResult::Failed;

			}

			else
			{
				//CANT FIND CREEP CAMPS
				return EBTNodeResult::Failed;
			}


		}
		// HERO AI PTR IS NULL
		return EBTNodeResult::Failed;
	}
	// IF TARGET CAMP HAS NOT BEEN CAPTURED YET
	return EBTNodeResult::Succeeded;
}


