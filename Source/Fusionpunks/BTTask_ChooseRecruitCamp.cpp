// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "HeroBase.h"
#include "CreepCamp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HeroStats.h"
#include "BTTask_ChooseRecruitCamp.h"

EBTNodeResult::Type UBTTask_ChooseRecruitCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AHeroAIController* heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	nextCaptureObjective = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));

	if (currSituation == ESituation::SE_CapturingUnsafeCamp || (currSituation == ESituation::SE_EngagingEnemyHero && OwnerComp.GetBlackboardComponent()->GetValueAsBool("ShouldRecruit")))
	{
		if (heroAI != nullptr && hero != nullptr)
		{
			TArray<ACreepCamp*> ownedCreepCamps = heroAI->GetSortedOwnedCampList();

			if (ownedCreepCamps.Num() > 0)
			{
				for (int32 i = 0; i < ownedCreepCamps.Num(); i++)
				{
					if (!ownedCreepCamps[i]->HasBeenRecruitedFrom() && ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp > 0)
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsInt("NumCreepsToRecruit",
							ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp);
						OwnerComp.GetBlackboardComponent()->SetValueAsObject("RecruitCamp", ownedCreepCamps[i]);
						UE_LOG(LogTemp, Error, TEXT("Found Recruit Camp unsafe or engaging enemy hero"));
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
				if (ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp > 0)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsInt("NumCreepsToRecruit",
						ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("RecruitCamp", ownedCreepCamps[i]);
					UE_LOG(LogTemp, Error, TEXT("Enough Creeps To Recruit From... Num Creeps at Camp %d, in Army %d"), ownedCreepCamps[i]->GetNumOfCreepsAtCamp(), hero->GetArmySize());
					return EBTNodeResult::Succeeded;
				}

			}
			return EBTNodeResult::Failed;

		}
		return EBTNodeResult::Failed;
	}


	else if (currSituation == ESituation::SE_AfterCapturing)
	{
		TArray<ACreepCamp*> ownedCreepCamps = heroAI->GetSortedOwnedCampList();

		if (ownedCreepCamps.Num() > 0)
		{
			for (int32 i = 0; i < ownedCreepCamps.Num(); i++)
			{
				if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool("GoingForWin") && !ownedCreepCamps[i]->HasBeenRecruitedFrom() && ownedCreepCamps[i]->GetNumOfCreepsAtCamp() - allowedNumCreepsLeftAtCamp > 0 &&
					hero->GetDistanceTo(ownedCreepCamps[i]) <= hero->GetDistanceTo(nextCaptureObjective))
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
	else
	{
		return EBTNodeResult::Failed;
	}
}


