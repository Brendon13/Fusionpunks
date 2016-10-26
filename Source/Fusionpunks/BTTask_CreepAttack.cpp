// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "HeroBase.h"
#include "Creep.h"
#include "BTTask_CreepAttack.h"


EBTNodeResult::Type UBTTask_CreepAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboardComponent = OwnerComp.GetBlackboardComponent();
	ACreep* owner = Cast<ACreep>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("SelfActor"));
	if (blackboardComponent && owner)
	{
		AHeroBase* enemy = Cast<AHeroBase>(blackboardComponent->GetValueAsObject("EnemyTarget"));
		ACreep* enemyCreep = Cast<ACreep>(blackboardComponent->GetValueAsObject("EnemyTarget"));
		if (enemy != nullptr)
		{
			/*Brendon - Note: damageEvent can be: FPointDamageEvent, FRadialDamageEvent, FDamageEvent or a custom DamageEvent
			more info @ https://www.unrealengine.com/blog/damage-in-ue4*/
			FDamageEvent damageEvent;
			float damage = owner->MeleeAttack();
			if(damage > 0)
				enemy->TakeDamage(damage, damageEvent, owner->GetController(), owner);
			//UE_LOG(LogTemp, Warning, TEXT("Took Damage From Creep"));

			return EBTNodeResult::Succeeded;
		}
		else if (enemyCreep != nullptr && !enemyCreep->GetBIsDead())
		{
			FDamageEvent damageEvent;
			enemyCreep->TakeDamage(owner->MeleeAttack(), damageEvent, owner->GetController(), owner);

			return EBTNodeResult::Succeeded;
		}
	}
	blackboardComponent->SetValueAsBool("hasWaited", false);
	return EBTNodeResult::Failed;
}


