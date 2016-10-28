// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HeroAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Creep.h"
#include "BTTask_DecideHowToApproachHero.h"


EBTNodeResult::Type UBTTask_DecideHowToApproachHero::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	hero = Cast<AHeroBase>(OwnerComp.GetAIOwner()->GetPawn());
	heroAI = Cast<AHeroAIController>(OwnerComp.GetAIOwner());
	if (hero != nullptr && heroAI != nullptr)
	{
		heroStats = hero->GetHeroStats();
		attackTarget = Cast<AHeroBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));
		campTarget = Cast<ACreepCamp>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("CampTarget"));
		healingWell = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("HealingWell"));
		if (attackTarget != nullptr && campTarget!= nullptr)
		{

			if (OwnerComp.GetAIOwner()->GetPawn()->ActorHasTag("Cyber"))
				teamCampType = ECampType::CT_Cyber;
			else
				teamCampType = ECampType::CT_Diesel;

			if (hero->GetCurrentHealth() <= healthPercentRequired)
			{
				approachStatus = EApproachStatus::AS_EscapingToBase;
			
			}

			else if ((hero->IsCapturing() || hero->GetDistanceTo(campTarget) <= 1200) && campTarget->GetCampType() != teamCampType)
			{	
				//OwnerComp.GetBlackboardComponent()->SetValueAsObject("HeroAttackSituationTarget", campTarget);
				approachStatus = EApproachStatus::AS_DefendingCamp;
				UE_LOG(LogTemp, Error, TEXT("Defensive State"));
				
			}
			else if ( attackTarget->GetArmySize() - hero->GetArmySize() <= creepDifferenceAllowed
				&& attackTarget->GetPlayerHealthAsDecimal() - hero->GetPlayerHealthAsDecimal() <= healthPercentDifferenceAllowed
				&& attackTarget->GetLevel() - hero->GetLevel() <= levelDifferenceAllowed )
			{			
				approachStatus = EApproachStatus::AS_AgressiveChase;	
				UE_LOG(LogTemp, Error, TEXT("Agressive State"));
			}

			else
			{
				approachStatus = EApproachStatus::AS_EscapingToNextCamp;
				UE_LOG(LogTemp, Error, TEXT("Escaping to next camp State"));
				
			}
			bNotifyTick = true;
			return EBTNodeResult::InProgress;
			
		}
		UE_LOG(LogTemp, Error, TEXT("Targets are null"));
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cant Find Hero/Hero AI in Decide how to approach"));
		return EBTNodeResult::Failed;
	}


}

void UBTTask_DecideHowToApproachHero::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (approachStatus == EApproachStatus::AS_DefendingCamp)
	{
		if (hero->GetPlayerHealthAsDecimal() <= healthPercentRequired)
		{
			UE_LOG(LogTemp, Error, TEXT("Health to low to engage"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
		

		else if (campTarget->GetCampType() == teamCampType)
		{
			//bNotifyTaskFinished = true;
			
			UE_LOG(LogTemp, Error, TEXT("AI Successfully Captured Camp from defensive state."));
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("ReachedCamp", false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool("CapturedCamp", true);
			heroAI->ResetAllCampsSafetyStatus();
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		else if (attackTarget->IsCapturing() || attackTarget->GetDistanceTo(campTarget)<=1000 || attackTarget->GetPlayerHealthAsDecimal() <= 0.2f)
		{
			if (hero->CheckForNearbyEnemyHero())
			{
				if (hero->GetDistanceTo(attackTarget) >= 300)
				{
					OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 50, false, true, false);
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("IN RANGE OF ENEMY HERO"));
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			}

			else
			{
				if (hero->GetDistanceTo(campTarget) >= 600)
				{
					OwnerComp.GetAIOwner()->MoveToActor(campTarget, 500, true, true, false);
				}
				else
				{
					//UE_LOG(LogTemp, Error, TEXT("IN RANGE OF DEFENSIVE CAMP"));
					if (hero->CheckForNearbyEnemyHero())
					{
						FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), attackTarget->GetActorLocation());
						lookAtTargetRotation.Pitch = 0;
						hero->SetActorRotation(lookAtTargetRotation);
					}
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}

			}
		}
		else
		{
			if (hero->GetDistanceTo(campTarget) >= 600)
			{
				OwnerComp.GetAIOwner()->MoveToActor(campTarget, 500, true, true, false);
			}
			else
			{
				if (hero->CheckForNearbyEnemyHero())
				{
					FRotator lookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(hero->GetActorLocation(), attackTarget->GetActorLocation());
					lookAtTargetRotation.Pitch = 0;
					hero->SetActorRotation(lookAtTargetRotation);
				}
				//UE_LOG(LogTemp, Error, TEXT("IN RANGE OF DEFENSIVE CAMP"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
			
		}

	}
	else if (approachStatus == EApproachStatus::AS_AgressiveChase)
	{


		if (hero->CheckForNearbyEnemyHero())
		{
			if (hero->GetDistanceTo(attackTarget) >= 300)
			{
				OwnerComp.GetAIOwner()->MoveToActor(attackTarget, 50, false, true, false);
			}
			else
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}

		else
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
	
	}

	else if (approachStatus == EApproachStatus::AS_EscapingToNextCamp)
	{
		if(hero->CheckForNearbyEnemyHero())
			OwnerComp.GetAIOwner()->MoveToActor(healingWell, 50, false, true, false);
		else
		{
			heroAI->ResetAllCampsSafetyStatus();
			campTarget->SetCampSafety(false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		}
	}
}