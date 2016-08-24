// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "DieselTower.h"


// Sets default values
ADieselTower::ADieselTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADieselTower::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADieselTower::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bHasSpawed && currProjectile->IsActorBeingDestroyed())
	{
		bHasSpawned = false;
	}

	spawnTimer += DeltaTime;
	if (enemyUnits.Num() > 0 && !bHasSpawed && spawnTimer >= 1)
	{
		if (enemyUnits[0]->IsA(ACharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Log, TEXT("Attacking Player!"));
			SpawnProjectile();

			if (currProjectile)
			{
				currProjectile->SetTarget(enemyUnits[0]);
			}
		}
	}

}

void ADieselTower::SpawnProjectile()
{
	if (whatToSpawn != NULL)
	{
		UWorld* const world = GetWorld();
		if (world && !bHasSpawed)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector spawnLocation = GetActorLocation();
			spawnLocation.Z = 350;


			currProjectile = world->SpawnActor<AProjectile>(whatToSpawn, spawnLocation, FRotator(0, 0, 0), SpawnParams);
			bHasSpawned = true;
			spawnTimer = 0;
		}
	}
}

