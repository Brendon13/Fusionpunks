// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToCamp.generated.h"

/**
 * 
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EReasonForGoingToCamp : uint8
{
	RGC_Capturing UMETA(DisplayName = "CapturingCamp"),
	RGC_Recruiting UMETA(DisplayName = "RecruitingCamp"),

};

UCLASS()
class FUSIONPUNKS_API UBTTask_MoveToCamp : public UBTTask_MoveTo
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	class ACreepCamp* targetCamp;
	class AHeroBase* hero;
	class AHeroAIController* heroAI;
	class HeroStats* heroStats;
	bool neutralCampExists;

	UPROPERTY(EditAnywhere)
		EReasonForGoingToCamp campGoal;
	UPROPERTY(EditAnywhere)
		float healthPercentageAbort = 0.2f;
	
};
