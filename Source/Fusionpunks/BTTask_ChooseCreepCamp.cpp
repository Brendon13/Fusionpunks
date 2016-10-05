// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "CreepCamp.h"
#include "BTTask_ChooseCreepCamp.h"

EBTNodeResult::Type UBTTask_ChooseCreepCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AHeroAIController* heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (heroAI!= nullptr) 
	{
		TArray<ACreepCamp*> creepCamps = heroAI->GetCreepCampList();
		ACreepCamp* targetCamp = nullptr;
		if (creepCamps.Num() > 0) 
		{
			for (int32 i = 0; i < creepCamps.Num(); i++) 
			{
				if (creepCamps[i]->GetCampType() == ECampType::CT_Neutral)
				{
					targetCamp = creepCamps[i];
					break;
				}
			}
			
			if (targetCamp != nullptr)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("CampTarget", targetCamp);
				return EBTNodeResult::Succeeded;
			}
		    return EBTNodeResult::Failed;
			
		}

		else 
		{
			return EBTNodeResult::Failed;
		}


	}

	return EBTNodeResult::Failed;
}


