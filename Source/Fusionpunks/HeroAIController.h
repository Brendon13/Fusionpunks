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

	void ResetCampsSafetyStatus()
	{
		for (int32 i = 0; i < creepCampList.Num(); i++)
		{
			creepCampList[i]->SetCampSafety(true);
		}
	}

	void ResetCampsRecruitedStatus()
	{
		for (int32 i = 0; i < creepCampList.Num(); i++)
		{
			creepCampList[i]->SetCampRecruitedStatus(false);
		}
	}

	void EmptyList() 
	{
		creepCampList.Empty();
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
	class AHeroBase* hero;
	virtual void Possess(APawn* Pawn) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	TArray<ACreepCamp*> GetCreepCampList();
	TArray<ACreepCamp*> GetSortedOwnedCampList();

	

	void LinkEnemyBaseProps(class ABase* base);

	//virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;


	//UFUNCTION(BlueprintCallable, Category = Components)
		//UBlackboardComponent* GetBlackboardComponent();
	void ResetAITreeTaskStatus();
	void ResetAllCampsSafetyStatus();
	void ResetAllCampsRecruitStatus();
	void RestartHeroAITree();
	bool CheckCampBeingAttacked();
	bool SafetyCheck(class ACreepCamp*);
	FORCEINLINE class ACreepCamp* GetCampBeingAttacked() const  { return campBeingAttacked; }
	FORCEINLINE int32 GetNumOwnedCamps() { return GetSortedOwnedCampList().Num(); }
	
protected:

	

	class AHeroBase* enemyHero;

	class ABase* enemyBase;

	class ACreepCamp* campBeingAttacked;

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
		FPriorityList campOwnedPriorityList;
		


	UPROPERTY(EditDefaultsOnly)
			TSubclassOf<ACreepCamp> creepCampClass;
	
	UPROPERTY(EditDefaultsOnly, Category = HomeBase)
		TSubclassOf<class AHealingWell> baseClass;


	
};
