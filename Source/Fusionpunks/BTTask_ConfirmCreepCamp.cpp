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
	 targetCamp = Cast<ACreepCamp> (OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
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

	if (targetCamp->GetCampType() == teamCampType)
	{
		//bNotifyTaskFinished = true;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		UE_LOG(LogTemp, Warning, TEXT("AI Successfully Captured Camp."));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
		heroAICont->ResetAllCampsSafetyStatus();
	}

	else if (heroStats->GetHealthPercent() < 0.15f)
	{
		//UE_LOG(LogTemp, Display, TEXT("AI HAS LOW HP WHILE HEADING TO CAMP"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	}

	else if (hero->CheckForNearbyEnemyCreeps() || hero->CheckForNearbyEnemyHero())
	{
		UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE Capturing Camp"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);


	}

	
}

void UBTTask_ConfirmCreepCamp::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
	

}