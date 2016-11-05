// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CreepCamp.h"
#include "BTTask_ConfirmCreepCamp.generated.h"

/**
 * 
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETypeOfCamp : uint8
{
	TC_CaptureCamp UMETA(DisplayName = "CaptureCamp"),
	TC_DefendCamp UMETA(DisplayName = "DefendCamp")


};
UCLASS()
class FUSIONPUNKS_API UBTTask_ConfirmCreepCamp : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult) override;

    class ACreepCamp* targetCamp;
	ECampType teamCampType;
	class AHeroAIController* heroAICont;
	class AHeroBase* hero;
	class HeroStats* heroStats;

	UPROPERTY(EditAnywhere)
		ETypeOfCamp campType;
};
