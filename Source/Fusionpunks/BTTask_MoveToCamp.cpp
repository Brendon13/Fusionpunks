// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include"HeroBase.h"
#include"HeroStats.h"
#include "BTTask_MoveToCamp.h"

EBTNodeResult::Type UBTTask_MoveToCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
		return EBTNodeResult::Succeeded;

	if (hero != nullptr)
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

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("ReachedCamp"))
	{
		return FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	if (hero->GetDistanceTo(targetCamp) < 650)
	{
		UE_LOG(LogTemp, Error, TEXT("TOO CLOSE TO CAMP"));
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", true);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	
	else if (heroStats->GetHealthPercent() < 0.15f)
	{
		UE_LOG(LogTemp, Display, TEXT("AI HAS LOW HP WHILE HEADING TO CAMP"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		
	}
	
	else if (hero->CheckForNearbyEnemyCreeps() ||hero->CheckForNearbyEnemyHero())
	{
		UE_LOG(LogTemp, Display, TEXT("AI SENSES ENEMY WHILE HEADING TO CAMP"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		

	}



	
}
