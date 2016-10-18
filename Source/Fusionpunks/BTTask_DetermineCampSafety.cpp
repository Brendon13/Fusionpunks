// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HeroStats.h"
#include "CreepCamp.h"
#include "BTTask_DetermineCampSafety.h"

EBTNodeResult::Type UBTTask_DetermineCampSafety::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);
	AHeroBase* hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	
	ACreepCamp* targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
	if (hero != nullptr && targetCamp!= nullptr)
	{
		HeroStats* heroStats = hero->GetHeroStats();
		heroStats->UpdateStats();
		if (heroStats->GetHealthPercent() >= healthPercentage && 
			 targetCamp->GetNumOfCreepsAtCamp()- heroStats->GetArmySize() <= allowedCreepDifference )
		{
			
			return EBTNodeResult::Succeeded;
		}
		targetCamp->SetCampSafety(false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}





