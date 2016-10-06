// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CreepCamp.h"
#include "Perception/PawnSensingComponent.h"

#include "HeroAIController.generated.h"


USTRUCT(BlueprintType)
struct FPriorityList
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<ACreepCamp*> creepCampList;
		
	FPriorityList()
	{
		
	}

	

	void GeneratePriorityList(TArray<class ACreepCamp*> creepCamps, AActor* owner)
	{
		for (int32 i = 0; i < creepCamps.Num(); i++)
		{
			creepCamps[i]->SetDistanceValue(owner->GetDistanceTo(creepCamps[i]));
			creepCampList.Add(creepCamps[i]);
			
		}
		creepCampList.Sort(&FPriorityList::ConstPredicate);
		
	}

	void SortByDistance(AActor* owner)
	{
		for (int32 i = 0; i < creepCampList.Num(); i++)
		{
			creepCampList[i]->SetDistanceValue(owner->GetDistanceTo(creepCampList[i]));
		}
		creepCampList.Sort(&FPriorityList::ConstPredicate);

	}

	void DisplayDistances() 
	{
		for (int32 i = 0; i < creepCampList.Num(); i++)
		{
			UE_LOG(LogTemp, Log, TEXT("Distance from camp %d is %f."), i, creepCampList[i]->GetDistanceValue());

		}
	}
	static bool ConstPredicate(const ACreepCamp& camp1, const ACreepCamp& camp2)
	{
		return (camp1.GetDistanceValue() <  camp2.GetDistanceValue());
	}

};


UCLASS()
class FUSIONPUNKS_API AHeroAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHeroAIController();
	virtual void Possess(APawn* Pawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	TArray<ACreepCamp*> GetCreepCampList();
	//virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;


	//UFUNCTION(BlueprintCallable, Category = Components)
		//UBlackboardComponent* GetBlackboardComponent();
	

protected:
	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly)
		UBlackboardComponent* BlackboardComponent;

	UPROPERTY(EditDefaultsOnly)
		UPawnSensingComponent* PawnSensingComponent;

		TArray<ACreepCamp*> creepCamps;
		
		FPriorityList campPriorityList;
	UPROPERTY(EditDefaultsOnly)
			TSubclassOf<ACreepCamp> creepCampClass;


	
};
