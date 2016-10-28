// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "HeroAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include"HeroBase.h"
#include"HeroStats.h"
#include "BTTask_MoveToCamp.h"

EBTNodeResult::Type UBTTask_MoveToCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	neutralCampExists = OwnerComp.GetBlackboardComponent()->GetValueAsBool("NeutralCampsExist");

	if (campGoal == EReasonForGoingToCamp::RGC_Capturing)
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
			return EBTNodeResult::Succeeded;
	}


	if (hero->CheckForNearbyInteractions())
	{
		//UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO CAPTURE CAMP"));
		//heroAI->ResetAITreeTaskStatus();
		return EBTNodeResult::Failed;
	}


	else if (hero != nullptr)
	{
		heroStats = hero->GetHeroStats();
		return EBTNodeResult::InProgress;	
	}

	else
	{
		return EBTNodeResult::Failed;
	}
	

}

void UBTTask_MoveToCamp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	heroStats->UpdateStats();
	

	if (heroStats->GetHealthPercent() < healthPercentageAbort)
	{
		//UE_LOG(LogTemp, Display, TEXT("AI HAS LOW HP WHILE HEADING TO CAMP"));
		heroAI->ResetAITreeTaskStatus();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	}

	else if (hero->IsCapturing())
	{

		UE_LOG(LogTemp, Error, TEXT("Capturing Camp"));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	}


	if (campGoal == EReasonForGoingToCamp::RGC_Capturing)
	{
		if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
		{
			return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}


		else if (hero->ActorHasTag("Cyber") && targetCamp->IsDieselCapturing() && neutralCampExists)
		{

			heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		}

		else if (hero->ActorHasTag("Diesel") && targetCamp->IsCyberCapturing() && neutralCampExists)

		{
			//UE_LOG(LogTemp, Display, TEXT("ENEMY PLAYER CAPTURING TARGET CAMP"));
			heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);


		}

		else if (hero->CheckForNearbyInteractions())
		{
			//UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO CAPTURE CAMP"));
			//heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		}
	}

	else if (campGoal == EReasonForGoingToCamp::RGC_Recruiting)
	{
	
		if (hero->CheckForNearbyEnemyHero())
		{
			//UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO RECURIT CAMP"));
			//heroAI->ResetAITreeTaskStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	}
	
}
