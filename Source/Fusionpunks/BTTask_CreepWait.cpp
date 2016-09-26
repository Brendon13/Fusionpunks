// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "BTTask_CreepWait.h"



void UBTTask_CreepWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if (MyMemory->RemainingWaitTime <= 0.0f)
	{
		UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
		
		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsBool("hasWaited", true);
		}
		
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
