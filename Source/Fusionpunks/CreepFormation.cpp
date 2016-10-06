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
	leaderFollowDistance = 400.0f;
	creepSeparationDistance = 200.0f;

	owningHero = Cast<AHeroBase>(GetOwner());
}

void UCreepFormation::BeginPlay()
{
	Super::BeginPlay();	
}

void UCreepFormation::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (owningHero)
	{
		const TArray<class ACreep*>& creepArmyArray = owningHero->GetCreepArmyArray();
		FVector targetPosition = FVector::ZeroVector;
		if (formationType == EFormationType::FT_Line)
		{
			for (int i = 0; i < creepArmyArray.Num(); i++)
			{
				targetPosition = FVector::ZeroVector;
				ACreepAIController* creepAiController = Cast <ACreepAIController>(creepArmyArray[i]->GetController());
				if(creepAiController)
				{
					//0 will always be the 'lead' creep to align with 
					if (i == 0)
					{
						/*FVector direction = (creepArmyArray[i]->GetActorLocation() - owningHero->GetActorLocation());
						direction.Normalize();
						FVector targetPosition = owningHero->GetActorLocation() - (direction * leaderFollowDistance);*/
						targetPosition = owningHero->GetActorLocation();
						targetPosition.X -= creepSeparationDistance;
						creepAiController->GetBlackboardComponent()->SetValueAsVector("FlockingPosition", targetPosition);
					}
					else if (i == 1)
					{
						targetPosition = creepArmyArray[0]->GetActorLocation();
						targetPosition.Y -= creepSeparationDistance;
						creepAiController->GetBlackboardComponent()->SetValueAsVector("FlockingPosition", targetPosition);
					}
					else
					{
						//even right odd left
						if (i % 2 == 0)
						{
							targetPosition = creepArmyArray[i - 2]->GetActorLocation();
							targetPosition.Y += creepSeparationDistance;
							creepAiController->GetBlackboardComponent()->SetValueAsVector("FlockingPosition", targetPosition);
						}
						else
						{
							targetPosition = creepArmyArray[i - 2]->GetActorLocation();
							targetPosition.Y -= creepSeparationDistance;
							creepAiController->GetBlackboardComponent()->SetValueAsVector("FlockingPosition", targetPosition);
						}
					}
				}
			}
		}
	}
	

}



