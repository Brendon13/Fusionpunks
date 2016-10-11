// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "HeroStats.h"
#include "BTTask_CheckHealth.h"

EBTNodeResult::Type UBTTask_CheckHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);
	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (hero != nullptr)
	{
		HeroStats* heroStats = hero->GetHeroStats();
		heroStats->UpdateStats();
		if (heroStats->GetHealthPercent() >= healthPercentage )
		{
			return EBTNodeResult::Succeeded;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("AI has too little hp"));

			return EBTNodeResult::Failed;
		}


	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero!"));
		return EBTNodeResult::Failed;
	}
}



