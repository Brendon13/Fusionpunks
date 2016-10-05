// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "BTTask_ConfirmCreepCamp.h"

EBTNodeResult::Type UBTTask_ConfirmCreepCamp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	ACreepCamp* targetCamp = Cast<ACreepCamp> (OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));

	if (targetCamp->GetCampType() == ECampType::CT_Neutral) 
	{
		return EBTNodeResult::Succeeded;
	}

	else 
	{
		return EBTNodeResult::Failed;
	}
}


