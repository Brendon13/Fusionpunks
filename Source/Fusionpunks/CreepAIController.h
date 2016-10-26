// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
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
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override; 

	UFUNCTION(BlueprintCallable, Category = Components)
		FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }

	UFUNCTION(BlueprintCallable, Category = AI)
	virtual void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result) override;

protected:
	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly)
		UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly)
		UPawnSensingComponent* PawnSensingComponent;

public:
	void RestartBehaviorTree();
	void StopBehaviorTree();

private:

};
