// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HeroStats.h"
#include "BTTask_ChooseRecruitCamp.h"

EBTNodeResult::Type UBTTask_ChooseRecruitCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AHeroAIController* heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (currSituation == ESituation::SE_CapturingUnsafeCamp)
	{
		if (heroAI != nullptr && hero != nullptr)
		{
			TArray<ACreepCamp*> ownedCreepCamps = heroAI->GetSortedOwnedCampList();
			
			if (ownedCreepCamps.Num() > 0 && ownedCreepCamps.Num() != 5)
			{
				for (int32 i = 0; i < ownedCreepCamps.Num(); i++)
				{
					if (!ownedCreepCamps[i]->HasBeenRecruitedFrom() && ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp > 0)
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsInt("NumCreepsToRecruit",
							ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp);
						OwnerComp.GetBlackboardComponent()->SetValueAsObject("RecruitCamp", ownedCreepCamps[i]);
						
						return EBTNodeResult::Succeeded;
					}

				}

				return EBTNodeResult::Failed;
			}
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;
	}

	else if (currSituation == ESituation::SE_NearbyCamp)
	{
		if (hero->CheckForNearbyOnwedCreepCamps())
		{
			TArray<ACreepCamp*> ownedCreepCamps = hero->GetNearbyOwnedCreepCamps();
			for (int32 i = 0; i < ownedCreepCamps.Num(); i++)
			{
				if (!ownedCreepCamps[i]->HasBeenRecruitedFrom() && ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp > 0)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsInt("NumCreepsToRecruit",
						ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("RecruitCamp", ownedCreepCamps[i]);
					
					return EBTNodeResult::Succeeded;
				}

			}

		}
	}


	return EBTNodeResult::Failed;
}


