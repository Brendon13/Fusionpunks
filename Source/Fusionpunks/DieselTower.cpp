// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "ProjectileTowerDamage.h"
#include "DieselTower.h"


// Sets default values
ADieselTower::ADieselTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	damage = 20.0f;
	damageEverySeconds = 1.0f;

	Tags.Add(TEXT("Diesel"));
}

// Called when the game starts or when spawned
void ADieselTower::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	towerDMG = GetWorld()->SpawnActor<AProjectileTowerDamage>(towerDMGPrefab, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
}

void ADieselTower::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::TriggerExit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (enemyUnits.Num() == 0)
	{
		if (IsValid(towerDMG))
		{
			towerDMG->PauseTimer();
		}
		
		bIsDealingDMG = false;
	}
}
void ADieselTower::CleanUp() 
{
	towerDMG->Destroy();
}

// Called every frame
void ADieselTower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (enemyUnits.Num() > 0)
	{
		if (enemyUnits[0]->IsA(ACharacter::StaticClass()))
		{
			//UE_LOG(LogTemp, Log, TEXT("Attacking Player!"));
			if (!bIsDealingDMG)
			{
				if (towerDMG->IsPaused())
				{
					towerDMG->UnPauseTimer(enemyUnits[0]);
				}
				else
				{
					towerDMG->StartTimer(damageEverySeconds, enemyUnits[0]);
				}
				bIsDealingDMG = true;
			}
		}
	}
	else 
	{
		if (bIsDealingDMG)
		{
			towerDMG->PauseTimer();
			bIsDealingDMG = false;
		}
	}

}

AProjectile* ADieselTower::SpawnProjectile()
{
	if (whatToSpawn != NULL)
	{
		UWorld* const world = GetWorld();
		if (world)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector spawnLocation = GetActorLocation();
			spawnLocation.Z += 200;


			currProjectile = world->SpawnActor<AProjectile>(whatToSpawn, spawnLocation, FRotator(0, 0, 0), SpawnParams);
			bHasSpawned = true;
			spawnTimer = 0;

			return currProjectile;
		}
		return NULL;
	}

	return NULL;
}

