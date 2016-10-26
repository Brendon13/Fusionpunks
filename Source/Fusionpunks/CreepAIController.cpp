// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepAIController.h"


ACreepAIController::ACreepAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnsensingComponent"));

	const ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("BehaviorTree'/Game/Creeps/AI/CreepBehaviorTree.CreepBehaviorTree'"));

	if (IsValid(BTFinder.Object))
	{
		BehaviorTreeAsset = BTFinder.Object;
	}
}


void ACreepAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	if (BehaviorTreeAsset)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		//UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent);
		BlackboardComponent->SetValueAsObject("SelfPawn", Pawn);
		BlackboardComponent->SetValueAsBool("bIsStunned", false);
		BlackboardComponent->SetValueAsBool("AtTargetPosition", true);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
	}
}

void ACreepAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateControlRotation(DeltaTime, false);
}

void ACreepAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);
}

void ACreepAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsBool("AtTargetPosition", true);
		BlackboardComponent->SetValueAsBool("hasWaited", false);
	}
}

void ACreepAIController::RestartBehaviorTree()
{
	BehaviorTreeComponent->RestartTree();
}

void ACreepAIController::StopBehaviorTree()
{
	BehaviorTreeComponent->StopTree();
}