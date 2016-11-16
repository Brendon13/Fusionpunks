// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HeroAIController.h"
#include "HeroStats.h"
#include "CreepCamp.h"
#include "BTTask_DetermineCampSafety.h"

EBTNodeResult::Type UBTTask_DetermineCampSafety::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);


	

	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	AHeroAIController* heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	ACreepCamp* targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("GoingForWin") || heroAI->GetNumOwnedCamps() == 5)
	{
		//UE_LOG(LogTemp, Error, TEXT("Going for win..Skip Choose!"));
		return EBTNodeResult::Succeeded;
	}
	if (hero != nullptr && targetCamp!= nullptr)
	{

		if (hero->ActorHasTag("Cyber"))
			enemyHero = targetCamp->GetDieselHero();
		else
			enemyHero = targetCamp->GetCyberHero();
		HeroStats* heroStats = hero->GetHeroStats();
		heroStats->UpdateStats();
		
		if(hero->ActorHasTag("Cyber") && targetCamp->GetCampType() == ECampType::CT_Cyber  || hero->ActorHasTag("Diesel") && targetCamp->GetCampType() == ECampType::CT_Diesel)
			return EBTNodeResult::Succeeded;


		if (heroStats->GetHealthPercent() >= healthPercentage && targetCamp->GetNumOfCreepsAtCamp() -  heroStats->GetArmySize() <= allowedCreepDifference)
		{
			bool neutralCampExists = false;
			TArray<ACreepCamp*> creepCamps = heroAI->GetCreepCampList();
			if (creepCamps.Num() > 0)
			{
				for (int32 i = 0; i < creepCamps.Num(); i++)
				{
					if (creepCamps[i]->GetCampType() == ECampType::CT_Neutral)

					{
						neutralCampExists = true;

					}
				}

				OwnerComp.GetBlackboardComponent()->SetValueAsBool("NeutralCampsExist", neutralCampExists);
				
				 if ((hero->ActorHasTag("Cyber") && targetCamp->IsDieselCapturing() && neutralCampExists && !OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp")
					&& !OwnerComp.GetBlackboardComponent()->GetValueAsBool("FoundNearbyEnemyCamp")))
				{  

					UE_LOG(LogTemp, Error, TEXT("Camp Flagged as Unsafe...Being Captured!"));
					targetCamp->SetCampSafety(false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
					return EBTNodeResult::Failed;
				}
			
				else if (hero->ActorHasTag("Diesel") && targetCamp->IsCyberCapturing() && neutralCampExists  && !OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp") &&
					!OwnerComp.GetBlackboardComponent()->GetValueAsBool("FoundNearbyEnemyCamp"))
				
				{
					UE_LOG(LogTemp, Error, TEXT("Camp Flagged as Unsafe...Being Captured!"));
					targetCamp->SetCampSafety(false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
					return EBTNodeResult::Failed;
				}


				else if (hero->ActorHasTag("Cyber") && targetCamp->IsDieselCapturing() &&
					(targetCamp->GetNumOfCreepsAtCamp() + enemyHero->GetArmySize()) - heroStats->GetArmySize() >= 5)
				{
					UE_LOG(LogTemp, Error, TEXT("Camp Flagged as Unsafe...Enemy Too Strong!"));
					targetCamp->SetCampSafety(false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("DefendingCamp", false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("FoundNearbyEnemyCamp", false);
					return EBTNodeResult::Failed;
				}

				else if (hero->ActorHasTag("Diesel") && targetCamp->IsCyberCapturing() &&
					(targetCamp->GetNumOfCreepsAtCamp() + enemyHero->GetArmySize()) - heroStats->GetArmySize() >= 5)
				{
					UE_LOG(LogTemp, Error, TEXT("Camp Flagged as Unsafe...Enemy Too Strong!"));
					targetCamp->SetCampSafety(false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("DefendingCamp", false);
					OwnerComp.GetBlackboardComponent()->SetValueAsBool("FoundNearbyEnemyCamp", false);
					return EBTNodeResult::Failed;
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("Camp Flagged as SAFE!"));
					return EBTNodeResult::Succeeded;
				}
			}
			return EBTNodeResult::Failed;
		
		}
		UE_LOG(LogTemp, Error, TEXT("Camp Flagged as Unsafe..Too many creeps!"));
		targetCamp->SetCampSafety(false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("DefendingCamp", false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("FoundNearbyEnemyCamp", false);
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}





