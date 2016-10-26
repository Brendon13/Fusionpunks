// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_AttackEnemyHero.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTask_AttackEnemyHero::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	target = Cast<AHeroBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
	isAttacking = false;
	if (hero != nullptr && target!=nullptr)
	{
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero or Target"));
		return EBTNodeResult::Failed;
	}
}
void UBTTask_AttackEnemyHero::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	
	if (!isAttacking)
	{
		GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackOnTimer, attackSpeed, true, 0);
		isAttacking = true;
		UE_LOG(LogTemp, Error, TEXT("Starting Attack Timer!"));
	}
	
	if (isAttacking)
	{
		FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), target->GetActorLocation());
		hero->SetActorRotation(lookAtTargetRotation);
	}
	
	if (hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(target) > 300 || target->GetCurrentHealth() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("STOP ATTACK TIMER"));
		target = nullptr;

		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
void UBTTask_AttackEnemyHero::AttackOnTimer()
{
	if (target!= nullptr)
		hero->Attack(target);
}

