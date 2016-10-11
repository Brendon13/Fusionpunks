// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "BTTask_MoveInRangeOfEnemy.h"


EBTNodeResult::Type UBTTask_MoveInRangeOfEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	return EBTNodeResult::InProgress;

}
void UBTTask_MoveInRangeOfEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

