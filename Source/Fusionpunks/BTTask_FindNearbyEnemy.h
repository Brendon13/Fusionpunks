// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNearbyEnemy.generated.h"

/**
 * 
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETargetEnemyType : uint8
{
	TE_Creeps UMETA(DisplayName = "Creeps"),
	TE_Hero UMETA(DisplayName = "Hero"),
	TE_Tower UMETA(DisplayName = "Tower")
};

UCLASS()
class FUSIONPUNKS_API UBTTask_FindNearbyEnemy : public UBTTaskNode
{
	GENERATED_BODY()


protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
		ETargetEnemyType enemyType;
};
