// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "AIController.h"
#include "HeroBase.h"
#include "HeroStats.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_ConfirmCreepCamp.h"


EBTNodeResult::Type UBTTask_ConfirmCreepCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	if (campType == ETypeOfCamp::TC_CaptureCamp)
		targetCamp = Cast<ACreepCamp> (OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
	else if(campType == ETypeOfCamp::TC_DefendCamp)
		targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("DefendCampTarget"));

	heroAICont = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
    hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (hero != nullptr && targetCamp != nullptr)
	{
		heroStats = hero->GetHeroStats();

		if (OwnerComp.GetAIOwner()->GetPawn()->ActorHasTag("Cyber"))
			teamCampType = ECampType::CT_Cyber;
		else
			teamCampType = ECampType::CT_Diesel;
	
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;

}


void UBTTask_ConfirmCreepCamp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//UE_LOG(LogTemp, Error, TEXT("Capture Perenctage: %f."), targetCamp->GetDieselCapturePercentage());
	if (targetCamp->GetCampType() == teamCampType && ((hero->ActorHasTag("Cyber") && targetCamp->GetCyberCapturePercentage() >= 1) || (hero->ActorHasTag("Diesel") && targetCamp->GetDieselCapturePercentage() >= 1)))
	{
		//bNotifyTaskFinished = true;
		
		UE_LOG(LogTemp, Error, TEXT("AI Successfully Captured Camp."));
		targetCamp->SetAIAbondonedCamp(false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("IsDefendingCamp", false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("FoundNearbyEnemyCamp", false);
		heroAICont->ResetAllCampsSafetyStatus();

		//testing
		heroAICont->ResetAllCampsRecruitStatus();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	else if (heroStats->GetHealthPercent() < 0.15f)
	{
		UE_LOG(LogTemp, Display, TEXT("AI HAS LOW HP WHILE HEADING TO CAMP"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	}

	else if (hero->CheckForNearbyInteractions())
	{
		UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE Capturing Camp"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);


	}
	else if (heroAICont->CheckCampBeingAttacked() && !heroAICont->GetCampBeingAttacked()->AIAbondonedCamp())
	{
		UE_LOG(LogTemp, Error, TEXT("Senses Camp Being Attacked"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	
}

void UBTTask_ConfirmCreepCamp::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	

}