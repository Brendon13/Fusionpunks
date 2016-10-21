// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RecruitCreeps.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBTTask_RecruitCreeps : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	class AHeroBase* hero;
	class AHeroAIController* heroAI;
	int32 numCreepsToRecruit;
	int32 numCreepsRecruited = 0;
	bool finishedRecruiting = false;
	bool isRecruiting = false;
	//class ACreep* target;
	FTimerHandle recruitTimerHandle;
	UPROPERTY(EditAnywhere)
		float recruitSpeed = 0.50f;
	UFUNCTION()
		void RecruitOnTimer();
	
};
