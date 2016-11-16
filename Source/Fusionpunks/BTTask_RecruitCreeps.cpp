// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroAIController.h"
#include "HeroBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_RecruitCreeps.h"


EBTNodeResult::Type UBTTask_RecruitCreeps::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr)
	{
		numCreepsToRecruit = OwnerComp.GetBlackboardComponent()->GetValueAsInt("NumCreepsToRecruit");
		
		numCreepsRecruited = 0;
		finishedRecruiting = false;
		isRecruiting = false;
		bNotifyTick = true;
		return EBTNodeResult::InProgress;
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero"));
		return EBTNodeResult::Failed;
	}
}

void UBTTask_RecruitCreeps::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!isRecruiting)
	{
		GetWorld()->GetTimerManager().SetTimer(recruitTimerHandle, this, &UBTTask_RecruitCreeps::RecruitOnTimer, recruitSpeed, true, 0);
		isRecruiting = true;
		UE_LOG(LogTemp, Error, TEXT("Starting Recruit Timer!"));
	}
	else if (finishedRecruiting)
	{
		UE_LOG(LogTemp, Error, TEXT("Finished Recruiting!"));
		heroAI->ResetAITreeTaskStatus();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	

}
void UBTTask_RecruitCreeps::RecruitOnTimer()
{
	if (hero)
	{
		hero->RecruitCreep();
		numCreepsRecruited++;
	}
	
	if (numCreepsRecruited == numCreepsToRecruit) 
	{
		if (recruitTimerHandle.IsValid())
			GetWorld()->GetTimerManager().ClearTimer(recruitTimerHandle);
		finishedRecruiting = true;
	}
}

