// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepAIController.h"


ACreepAIController::ACreepAIController()
{
	/*BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	const ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT(""));

	if (IsValid(BTFinder.Object))
	{

	}*/
}


void ACreepAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
}