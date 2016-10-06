// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "BTTask_MoveToCamp.h"



void UBTTask_MoveToCamp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACreepCamp* targetCamp = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));

	if (targetCamp->GetCampType() != ECampType::CT_Neutral)
	{
		AbortTask(OwnerComp, NodeMemory);
	}

	
}
