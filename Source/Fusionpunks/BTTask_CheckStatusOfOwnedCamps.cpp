// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "HeroBase.h"
#include "CreepCamp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HeroStats.h"
#include "BTTask_CheckStatusOfOwnedCamps.h"



EBTNodeResult::Type UBTTask_CheckStatusOfOwnedCamps::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController> (OwnerComp.GetAIOwner());
	
	if (hero != nullptr && heroAI != nullptr)
	{

		otherCampObjective = Cast<ACreepCamp>
			(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));

		if (heroAI->CheckCampBeingAttacked())
		{
			campBeingAttacked = heroAI->GetCampBeingAttacked();

			if (campBeingAttacked != nullptr)
			{
				if (hero->ActorHasTag("Cyber"))
					enemyHero = campBeingAttacked->GetDieselHero();
				else
					enemyHero = campBeingAttacked->GetCyberHero();

				if (campBeingAttacked->GetNumOfCreepsAtCamp() >= numCreepsAlwaysDefend && (enemyHero->GetArmySize() - hero->GetArmySize() <= creepDifferenceAllowed
					&& enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() <= healthPercentDifferenceAllowed
					&& enemyHero->GetLevel() - hero->GetLevel() <= levelDifferenceAllowed))
				{
					//Too Many Creeps At Camp...Defend
					UE_LOG(LogTemp, Error, TEXT("Too Many Creeps At Camp...Defend"));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("DefendCampTarget", campBeingAttacked);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDefendingCamp", true);
					return  EBTNodeResult::Succeeded;
				}

				else if (hero->IsCapturing() && ((hero->ActorHasTag("Cyber") && otherCampObjective->GetCyberCapturePercentage() >= ignoreDefendPercentage &&
					otherCampObjective->GetCyberCapturePercentage() < 1)|| 
					(hero->ActorHasTag("Diesel") && otherCampObjective->GetDieselCapturePercentage() >= ignoreDefendPercentage  &&
						otherCampObjective->GetDieselCapturePercentage() < 1)))
				{
					//About to capture a camp..Not Defend
					UE_LOG(LogTemp, Error, TEXT("Not Defend because percentage captured = %f"), otherCampObjective->GetDieselCapturePercentage());

					campBeingAttacked->SetAIAbondonedCamp(true);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDefendingCamp", false);
					return  EBTNodeResult::Failed;
				}

				if (hero->GetDistanceTo(campBeingAttacked) <= hero->GetDistanceTo(otherCampObjective)
					&& (enemyHero->GetArmySize() - hero->GetArmySize() <= creepDifferenceAllowed
					&& enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() <= healthPercentDifferenceAllowed
					&& enemyHero->GetLevel() - hero->GetLevel() <= levelDifferenceAllowed))
				{

					//AI THINKS IT CAN DEFEND AGAINST ENEMY HERO
					UE_LOG(LogTemp, Error, TEXT("Safe or close enough to defend"));
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("DefendCampTarget", campBeingAttacked);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDefendingCamp", true);
					return  EBTNodeResult::Succeeded;


				}

				// NOT SAFE OR TOO FAR
				UE_LOG(LogTemp, Error, TEXT("NOT SAFE TO DEFEND CAMP"));
				campBeingAttacked->SetAIAbondonedCamp(true);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDefendingCamp", false);
				return  EBTNodeResult::Failed;


			}


			//attacked camp is null
			return  EBTNodeResult::Failed;

		}


		else 
		{
			//NO CAMPS BEING ATTACKED YO
			return  EBTNodeResult::Failed;
		}







	}
	//HERO OR HEROAI NULL
	return  EBTNodeResult::Failed;








}
