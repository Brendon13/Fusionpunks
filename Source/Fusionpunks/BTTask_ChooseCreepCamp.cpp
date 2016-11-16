// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "CreepCamp.h"
#include "BTTask_ChooseCreepCamp.h"

EBTNodeResult::Type UBTTask_ChooseCreepCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AHeroAIController* heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	//UE_LOG(LogTemp, Error, TEXT("CHOOSING CAMP"));
	

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
	{
		//UE_LOG(LogTemp, Error, TEXT("Already Reached Camp..Skip Choose!"));
		return EBTNodeResult::Succeeded;
	}

	
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("FoundNearbyEnemyCamp"))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("CampTarget", OwnerComp.GetBlackboardComponent()->GetValueAsObject("NearbyEnemyCamp"));
	    //UE_LOG(LogTemp, Error, TEXT("Found Nearby Enemy Camp..Skip Choose!!"));
		return EBTNodeResult::Succeeded;
	}
	
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("GoingForWin"))
	{
		//UE_LOG(LogTemp, Error, TEXT("Going for win..Skip Choose!"));
		return EBTNodeResult::Succeeded;
	}

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsDefendingCamp"))
	{	
		//UE_LOG(LogTemp, Error, TEXT("Hero is defending a camp..Skip Choose!"));
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("CampTarget", OwnerComp.GetBlackboardComponent()->GetValueAsObject("DefendCampTarget"));
		return EBTNodeResult::Succeeded;
	}



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
				//	UE_LOG(LogTemp, Error, TEXT("Found A Safe Camp!"));
					return EBTNodeResult::Succeeded;
				}

				// TARGET CAMP IS NULL 
				//UE_LOG(LogTemp, Error, TEXT("NO SAFE CAMPS!"));
				return EBTNodeResult::Failed;

			}

			else
			{
				//CANT FIND CREEP CAMPS
				//UE_LOG(LogTemp, Error, TEXT("CANT FIND CREEP CAMPS!"));


				return EBTNodeResult::Failed;
			}


		}
		// HERO AI PTR IS NULL
		//UE_LOG(LogTemp, Error, TEXT("HERO AI PTR IS NULL!"));
		return EBTNodeResult::Failed;
	}
	// IF TARGET CAMP HAS NOT BEEN CAPTURED YET
	//UE_LOG(LogTemp, Error, TEXT("TARGET CAMP HAS NOT BEEN CAPTURED YET!"));
	return EBTNodeResult::Succeeded;
}


