// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creep.h"
#include "BTTask_CheckCreepStatus.h"


EBTNodeResult::Type UBTTask_CheckCreepStatus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	creepTarget = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
	campTarget = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
	TArray<ACreep*> creepsAtCamp = campTarget->GetCreepArray();
	
	if (creepsAtCamp.Contains(creepTarget)|| !creepTarget->bBelongsToCamp || OwnerComp.GetBlackboardComponent()->GetValueAsBool("GoingForWin"))
		return EBTNodeResult::Succeeded;
	else
		return EBTNodeResult::Failed;


}

