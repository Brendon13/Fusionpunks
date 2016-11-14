// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "CreepCamp.h"
#include "Base.h"
#include "HealingWell.h"
#include "HeroAIController.h"

AHeroAIController::AHeroAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnsensingComponent"));
	
	const ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("BehaviorTree'/Game/Heroes/AI/HeroAI_Tree.HeroAI_Tree'"));

	if (IsValid(BTFinder.Object))
	{
		BehaviorTreeAsset = BTFinder.Object;
	}
	bAttachToPawn = true;
	
}
void AHeroAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
	
	if (BehaviorTreeAsset)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		//UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComponent);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
		
		
	}
}

void AHeroAIController::BeginPlay() 
{
	Super::BeginPlay();
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), creepCampClass, actorList);
	for (int32 i = 0; i < actorList.Num(); i++)
	{
		creepCamps.Add(Cast<ACreepCamp>(actorList[i]));
	}
	UE_LOG(LogTemp, Log, TEXT("Found %d CreepCamps."), creepCamps.Num());
	campPriorityList.GeneratePriorityList(creepCamps, GetPawn());
	campPriorityList.DisplayDistances();

	
	
	TArray<AActor*> healingWells;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), baseClass,healingWells );
	if (healingWells.Num() > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Healing Well FOUND"));
		if(GetPawn()->ActorHasTag("Cyber") && healingWells[0]->ActorHasTag("Cyber"))
			BlackboardComponent->SetValueAsObject("HealingWell", healingWells[0]);
		else
			BlackboardComponent->SetValueAsObject("HealingWell", healingWells[1]);
		
	}
	BlackboardComponent->SetValueAsBool("ReachedCamp", false);
	BlackboardComponent->SetValueAsBool("CapturedCamp", true);
	BlackboardComponent->SetValueAsBool("IsDefendingCamp", false);
	BlackboardComponent->SetValueAsBool("FoundNearbyEnemyCamp", false);
	BlackboardComponent->SetValueAsBool("ShouldRecruit", false);
	BlackboardComponent->SetValueAsBool("GoingForWin", false);
	hero = Cast<AHeroBase>(GetPawn());
	
}

void AHeroAIController::RestartHeroAITree()
{
	
	BehaviorTreeComponent->RestartTree();
	BlackboardComponent->SetValueAsBool("GoingForWin", false);
}

void AHeroAIController::ResetAITreeTaskStatus() 
{
	BlackboardComponent->SetValueAsBool("ReachedCamp", false);
	BlackboardComponent->SetValueAsBool("CapturedCamp", true);
	BlackboardComponent->SetValueAsBool("IsDefendingCamp", false);
	BlackboardComponent->SetValueAsBool("FoundNearbyEnemyCamp", false);
	BlackboardComponent->SetValueAsBool("ShouldRecruit", false);
	ResetAllCampsRecruitStatus();
	ResetAllCampsSafetyStatus();
}


void AHeroAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	//UpdateControlRotation(DeltaTime, false);
	
}

TArray<ACreepCamp*> AHeroAIController::GetCreepCampList() 
{
	campPriorityList.SortByDistance(GetPawn());
	return campPriorityList.creepCampList;
}

void AHeroAIController::ResetAllCampsSafetyStatus() 
{
	campPriorityList.ResetCampsSafetyStatus();
}
TArray<ACreepCamp*> AHeroAIController::GetSortedOwnedCampList() 
{
	campOwnedPriorityList.EmptyList();
	campOwnedPriorityList.GeneratePriorityList(Cast<AHeroBase>(GetPawn())->GetCapturedCamps(), GetPawn());
	return campOwnedPriorityList.creepCampList;

}
void AHeroAIController::ResetAllCampsRecruitStatus()
{
	campPriorityList.ResetCampsRecruitedStatus();
}

bool AHeroAIController::SafetyCheck(ACreepCamp* camp) 
{
	if (hero->ActorHasTag("Cyber") && camp->IsDieselCapturing() && (camp->GetNumOfCreepsAtCamp() + enemyHero->GetArmySize()) - hero->GetArmySize() <= 5)
		return true;
	else if (hero->ActorHasTag("Diesel") && camp->IsCyberCapturing() &&
		(camp->GetNumOfCreepsAtCamp() + enemyHero->GetArmySize()) - hero->GetArmySize() <= 5)
		return true;
	else if (hero->ActorHasTag("Cyber") && !camp->IsDieselCapturing() && camp->GetNumOfCreepsAtCamp() - hero->GetArmySize() <= 3)
		return true;
	else if (hero->ActorHasTag("Diesel") && !camp->IsCyberCapturing() && camp->GetNumOfCreepsAtCamp() - hero->GetArmySize() <= 3)
		return true;
	else
		return false;
}


bool AHeroAIController::CheckCampBeingAttacked()
{

	campBeingAttacked = nullptr;
	campOwnedPriorityList.EmptyList();
	campOwnedPriorityList.GeneratePriorityList(Cast<AHeroBase>(GetPawn())->GetCapturedCamps(), GetPawn());
	if (campOwnedPriorityList.creepCampList.Num() > 0)
	{

		if (enemyHero == nullptr)
		{
			if (hero->ActorHasTag("Cyber"))
				enemyHero = campOwnedPriorityList.creepCampList[0]->GetDieselHero();
			else
				enemyHero = campOwnedPriorityList.creepCampList[0]->GetCyberHero();

		}

		for (int i = 0; i <campOwnedPriorityList.creepCampList.Num(); i++)
		{
			if (enemyHero->IsCapturing() && 
				enemyHero->GetCampBeingCaptured() == campOwnedPriorityList.creepCampList[i])
			{
				campBeingAttacked = campOwnedPriorityList.creepCampList[i];
				break;
			}
		

		}

		return campBeingAttacked != nullptr;
		

	}
	//no owned camps
	return false;
}

void AHeroAIController::LinkEnemyBaseProps(ABase* base) 
{
	enemyBase = base;
	BlackboardComponent->SetValueAsObject("EnemyBase", Cast<AActor>(base));
}