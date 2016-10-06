// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "HeroBase.h"
#include "CreepMoveToEnemy.h"


void UCreepMoveToEnemy::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	
	if (blackboardComponent)
	{
		blackboardComponent->SetValueAsBool("AtTargetPosition", true);
	}
}

