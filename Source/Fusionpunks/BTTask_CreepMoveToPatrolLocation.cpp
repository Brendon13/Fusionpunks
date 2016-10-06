// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "BTTask_CreepMoveToPatrolLocation.h"




void UBTTask_CreepMoveToPatrolLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	if (blackboardComponent)
	{
		if (blackboardComponent->GetValueAsBool("belongsToCamp") == false)
		{
			AbortTask(OwnerComp, NodeMemory);
		}
	}
}