// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "BTTask_HealAtHealingWell.h"

EBTNodeResult::Type UBTTask_HealAtHealingWell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UE_LOG(LogTemp, Error, TEXT("Healing At Well"));
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr)
	{
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
	}
	UE_LOG(LogTemp, Error, TEXT("Cant Find Hero Data"));
	return EBTNodeResult::Failed;
}
void UBTTask_HealAtHealingWell::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (hero->GetCurrentHealth() == hero->GetMaxHealth())
	{
		UE_LOG(LogTemp, Error, TEXT("FullyHealed"));
		heroAI->ResetAITreeTaskStatus();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

