// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "Creep.h"
#include "CreepAIController.h"
#include "CreepFormation.h"


UCreepFormation::UCreepFormation()
{
	bWantsBeginPlay = true;

	PrimaryComponentTick.bCanEverTick = true;

	formationType = EFormationType::FT_Line;

	owningHero = Cast<AHeroBase>(GetOwner());

	PrimaryComponentTick.bCanEverTick = false;

}

void UCreepFormation::BeginPlay()
{
	Super::BeginPlay();	
}

void UCreepFormation::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );


	
	


	//if (owningHero)
	//{
	//	const TArray<class ACreep*>& creepArmyArray = owningHero->GetCreepArmyArray();
	//	FVector targetPosition = FVector::ZeroVector;
	//	if (formationType == EFormationType::FT_Line)
	//	{
	//		for (int i = 0; i < creepArmyArray.Num(); i++)
	//		{
	//			targetPosition = FVector::ZeroVector;
	//			ACreepAIController* creepAiController = Cast <ACreepAIController>(creepArmyArray[i]->GetController());
	//			if(creepAiController)
	//			{
	//				//0 will always be the 'lead' creep to align with 
	//				if (i == 0)
	//				{
	//					targetPosition = owningHero->GetActorLocation();
	//					targetPosition.X -= creepSeparationDistance;
	//					creepAiController->GetBlackboardComponent()->SetValueAsVector("FormationPosition", targetPosition);
	//				}
	//				else if (i == 1)
	//				{
	//					targetPosition = creepArmyArray[0]->GetActorLocation();
	//					targetPosition.Y -= creepSeparationDistance;
	//					creepAiController->GetBlackboardComponent()->SetValueAsVector("FormationPosition", targetPosition);
	//				}
	//				else
	//				{
	//					//even right odd left
	//					if (i % 2 == 0)
	//					{
	//						targetPosition = creepArmyArray[i - 2]->GetActorLocation();
	//						targetPosition.Y += creepSeparationDistance;
	//						creepAiController->GetBlackboardComponent()->SetValueAsVector("FormationPosition", targetPosition);
	//					}
	//					else
	//					{
	//						targetPosition = creepArmyArray[i - 2]->GetActorLocation();
	//						targetPosition.Y -= creepSeparationDistance;
	//						creepAiController->GetBlackboardComponent()->SetValueAsVector("FormationPosition", targetPosition);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

void UCreepFormation::CalculateSlotPositions(int ArmySize)
{
	int armySize = ArmySize;

	if (armySize < slotArray.Num())
	{
		int diff = slotArray.Num() - armySize;
		for (int i = 0; i < diff; i++)
		{
			slotArray.Pop();
		}	
	}
	else
	{
		slotArray.SetNum(ArmySize);
	}

	switch (formationType)
	{
		case EFormationType::FT_Circle:
		{
			for (int i = 1; i <= armySize; i++)
			{ 
				float theta = 6.28f / armySize;
				float angle = theta * i;

				slotArray[i - 1] = FVector::FVector((circleRadius * FMath::Cos(angle)), (circleRadius * FMath::Sin(angle)), 0);
			}
		}break;

		case EFormationType::FT_Line:
		{
			//implement 
		}break;
	}

}




