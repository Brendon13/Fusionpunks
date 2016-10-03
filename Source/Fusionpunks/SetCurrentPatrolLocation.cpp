// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "SetCurrentPatrolLocation.h"



EBTNodeResult::Type USetCurrentPatrolLocation::AbortTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("Set Current Patrol Location Task Aborted!"));

	return EBTNodeResult::Aborted;
}

EBTNodeResult::Type USetCurrentPatrolLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	//if (BlackboardComp)
	//{
	//	//pick random direction
	//	FVector randomLocation = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), 0);
	//	if (Cast<ACreep>(BlackboardComp->GetValueAsObject("SelfPawn")))
	//	{
	//		ACreep* Creep = Cast<ACreep>(BlackboardComp->GetValueAsObject("SelfPawn"));
	//		randomLocation *= Creep->GetPatrolRadius();

	//		BlackboardComp->SetValueAsVector("PatrolPosition", randomLocation);
	//		return EBTNodeResult::Succeeded;
	//	}

	//	UE_LOG(LogTemp, Warning, TEXT("Set Current Patrol Location Task Self Pawn Cast as Creep Failed!"));
	//	return EBTNodeResult::Failed;
	//}
	//return EBTNodeResult::Failed;


	UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(OwnerComp.GetOwner());
	ACreepAIController* MyAIController = Cast<ACreepAIController>(OwnerComp.GetOwner());
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (NavSys && MyAIController && MyAIController->GetPawn() && BlackboardComponent)
	{
		const float SearchRadius = 500.0f;
		FNavLocation RandomPt;

		AActor* myCC = Cast<AActor>(BlackboardComponent->GetValueAsObject("CreepCampHome"));

		if (myCC != nullptr)
		{
			const bool bFound = NavSys->GetRandomReachablePointInRadius(myCC->GetActorLocation(), SearchRadius, RandomPt);
			if (bFound)
			{
				BlackboardComponent->SetValueAsVector("PatrolPosition", RandomPt.Location);
				BlackboardComponent->SetValueAsBool("AtTargetPosition", false);
				return EBTNodeResult::Succeeded;
			}
		}
		
	}

	return EBTNodeResult::Failed;
	
}

void USetCurrentPatrolLocation::OnTaskFinished(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	UE_LOG(LogTemp, Warning, TEXT("Set Current Patrol Location Task Finished!"));
}
