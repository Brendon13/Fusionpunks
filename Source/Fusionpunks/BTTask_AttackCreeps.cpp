// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AIController.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Creep.h"
#include "Kismet/KismetMathLibrary.h"
#include "BTTask_AttackCreeps.h"

EBTNodeResult::Type UBTTask_AttackCreeps::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	isAttacking = false;
	if (hero != nullptr)
	{
		heroStats = hero->GetHeroStats();
		target = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		if (target)
		{
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
		}
		UE_LOG(LogTemp, Error, TEXT("Cant Find Creep To Attack"));

		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero In Attack Creep Task"));
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
		if (isAttacking)
		{
				FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), target->GetActorLocation());
				lookAtTargetRotation.Pitch = 0;
				hero->SetActorRotation(lookAtTargetRotation);
		}
		
		if (target->GetHealthAsDecimal()<=0 || hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(target) > 300 )
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK TIMER"));
			target = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
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
	if(target!= nullptr)
		hero->Attack(target);
}