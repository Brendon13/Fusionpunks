// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AIController.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creep.h"
#include "BTTask_AttackCreeps.h"

EBTNodeResult::Type UBTTask_AttackCreeps::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (hero != nullptr)
	{
		heroStats = hero->GetHeroStats();
		target = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		if (target)
		{
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
		}
<<<<<<< HEAD
=======
		UE_LOG(LogTemp, Error, TEXT("Cant Find Creep To Attack"));
>>>>>>> refs/remotes/origin/Master-(Do-Not-Touch)
		return EBTNodeResult::Failed;

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero"));
		return EBTNodeResult::Failed;
	}

	
}

void UBTTask_AttackCreeps::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
		if (!isAttacking)
		{
				GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackCreeps::AttackOnTimer, attackSpeed, true, 0);
				isAttacking = true;
				UE_LOG(LogTemp, Error, TEXT("Starting Attack Timer!"));
		}

		if (isAttacking && target->IsActorBeingDestroyed())
		{
			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				isAttacking = false;
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}

		}
	


}

void UBTTask_AttackCreeps::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	if (attackTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);

}
void UBTTask_AttackCreeps::AttackOnTimer() 
{
	
	hero->Attack(target);
}