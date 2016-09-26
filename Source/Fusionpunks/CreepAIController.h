// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CreepAIController.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ACreepAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACreepAIController();
	virtual void Possess(APawn* Pawn) override;

protected:
	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly)
		UBlackboardComponent* BlackboardComponent;

};
