// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "HeroBase.h"
#include "Creep.h"
#include "BTTask_CreepAttack.h"


EBTNodeResult::Type UBTTask_CreepAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	ACreep* owner = Cast<ACreep>(OwnerComp.GetOwner());
	if (blackboardComponent && owner)
	{
		AActor* enemy = Cast<AActor>(blackboardComponent->GetValueAsObject("EnemyTarget"));

		if (Cast<AHeroBase>(enemy))
		{
			AHeroBase* enemyHero = Cast<AHeroBase>(enemy);
			/*Brendon - Note: damageEvent can be: FPointDamageEvent, FRadialDamageEvent, FDamageEvent or a custom DamageEvent
			more info @ https://www.unrealengine.com/blog/damage-in-ue4*/
			FDamageEvent damageEvent;
			enemyHero->TakeDamage(owner->GetDamage(), damageEvent, owner->GetController(), owner);
			UE_LOG(LogTemp, Warning, TEXT("Took Damage From Creep"));

			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;

	
}


void UBTTask_CreepAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	ACreep* owner = Cast<ACreep>(OwnerComp.GetOwner());
	if (owner)
	{
		//start attack cooldown timer

	}
}

