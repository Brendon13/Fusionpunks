// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "TowerBase.h"
#include "Creep.h"
#include "HeroAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_AttackEnemyHero.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type UBTTask_AttackEnemyHero::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemyHero = nullptr;
	enemyCreep = nullptr;
	enemyTower = nullptr;
	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(AHeroBase::StaticClass()))
	{
		enemyHero = Cast<AHeroBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_Hero;
	}
	else if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(ACreep::StaticClass()))
	{
		enemyCreep = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_Creep;
	}

	else if (OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")->IsA(ATowerBase::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Attack Target is a tower!"));
		enemyTower = Cast<ATowerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		targetType = ETargetType::TT_Tower;
	}
	OwnerComp.GetAIOwner()->StopMovement();
	isAttacking = false;
	if (hero != nullptr)
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
	if (targetType == ETargetType::TT_Hero && enemyHero != nullptr)
	{
		if (!isAttacking)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackHeroOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Hero Timer!"));
		}

		if (isAttacking)
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyHero->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if ((hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort && enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() > 0 &&
			enemyHero->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() > healthPercentageAbort) || hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning ||
			hero->GetDistanceTo(enemyHero) > 300 || enemyHero->bIsRespawning || enemyHero->GetPlayerHealthAsDecimal() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK Hero TIMER"));
			enemyHero = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				hero->SetHeroAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
	else if (targetType == ETargetType::TT_Creep && enemyCreep != nullptr)
	{

		if (!isAttacking && enemyCreep != nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackCreepOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Creep Timer!"));
		}

		if (isAttacking && enemyCreep != nullptr)
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyCreep->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if (hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(enemyCreep) > 300 || enemyCreep->GetHealthAsDecimal() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK Creep TIMER"));
			enemyCreep = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}

	else if (targetType == ETargetType::TT_Tower && enemyTower != nullptr)
	{
	
		if (!isAttacking && enemyTower != nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &UBTTask_AttackEnemyHero::AttackTowerOnTimer, attackSpeed, true, 0);
			isAttacking = true;
			UE_LOG(LogTemp, Error, TEXT("Starting Attack Tower Timer!"));
		}

		if (isAttacking && enemyTower != nullptr)
		{
			FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), enemyTower->GetActorLocation());
			lookAtTargetRotation.Pitch = 0;
			hero->SetActorRotation(lookAtTargetRotation);
		}

		if (hero->GetPlayerHealthAsDecimal() <= healthPercentageAbort || hero->GetDistanceTo(enemyTower) > 500 || enemyTower->GetHpPercent() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("STOP ATTACK TOWER TIMER"));
			enemyTower = nullptr;

			if (attackTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
				//hero->SetCreepAttacking(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}


	else if(targetType == ETargetType::TT_None)
	{
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

}
void UBTTask_AttackEnemyHero::AttackHeroOnTimer()
{
	if (hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning ||
		hero->GetDistanceTo(enemyHero) > 300 || enemyHero->bIsRespawning || enemyHero->GetPlayerHealthAsDecimal() <= 0)
	{
		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
		}
	}

	else if (enemyHero != nullptr)
		hero->Attack(enemyHero);
}
void UBTTask_AttackEnemyHero::AttackCreepOnTimer()
{
	if (hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning || hero->GetDistanceTo(enemyCreep) > 300 || enemyCreep->GetHealthAsDecimal() <= 0)
	{
		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
		}
	}
	else if (enemyCreep != nullptr)
			hero->Attack(enemyCreep);
	
}
void UBTTask_AttackEnemyHero::AttackTowerOnTimer()
{
	if (hero->GetPlayerHealthAsDecimal() <= 0 || hero->bIsRespawning || hero->GetDistanceTo(enemyTower) > 500 || enemyTower->GetHpPercent() <= 0)
	{
		if (attackTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
		}
	}
	else if (enemyTower != nullptr)
		hero->Attack(enemyTower);
}
