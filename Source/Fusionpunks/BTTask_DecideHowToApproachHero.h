// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DecideHowToApproachHero.generated.h"

/**
 * 
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EApproachStatus : uint8
{
	AS_DefendingCamp UMETA(DisplayName = "DefendingCamp"),
	AS_AgressiveChase UMETA(DisplayName = "AgressiveChase"),
	AS_Escaping UMETA(DisplayName = "Escaping")
};


UCLASS()
class FUSIONPUNKS_API UBTTask_DecideHowToApproachHero : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	 virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	class AHeroBase* hero;
	class HeroStats* heroStats;
	class AHeroBase* attackTarget;
	class ACreepCamp* campTarget;
	UPROPERTY(EditAnywhere)
		EApproachStatus approachStatus;
	UPROPERTY(EditAnywhere)
		int32 creepDifferenceAllowed = 3;
	UPROPERTY(EditAnywhere)
		float healthPercentRequired = 0.15f;
	

	
};
