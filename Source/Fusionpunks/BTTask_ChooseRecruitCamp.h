// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChooseRecruitCamp.generated.h"

/**
 * 
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESituation : uint8
{
	SE_CapturingUnsafeCamp UMETA(DisplayName = "CapturingUnsafeCamp"),
	SE_EngagingEnemyHero UMETA(DisplayName = "EngageUnsafeEnemy"),
	SE_NearbyCamp UMETA(DisplayName = "NearbyCamp"),
	SE_AfterCapturing UMETA(DisplayName = "AfterCapturing")
};

UCLASS()
class FUSIONPUNKS_API UBTTask_ChooseRecruitCamp : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	UPROPERTY(EditAnywhere, Category = RecruitConditions)
		int32 allowedNumCreepsLeftAtCamp;


	UPROPERTY(EditAnywhere, Category = RecruitConditions)
		ESituation currSituation;

	class ACreepCamp* nextCaptureObjective;
		

};
