// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "BTTask_CreepMoveToEnemy.h"



void UBTTask_CreepMoveToEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	if(blackboardComponent)
	{
		if (blackboardComponent->GetValueAsObject("EnemyTarget") == nullptr)
		{
			AbortTask(OwnerComp, NodeMemory);
		}
	}
}
