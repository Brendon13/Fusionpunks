// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "TowerBase.h"
#include "DieselTower.generated.h"

UCLASS()
class FUSIONPUNKS_API ADieselTower : public ATowerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADieselTower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
private:
	void SpawnProjectile();

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class AProjectile> whatToSpawn;

	AProjectile* currProjectile;

	bool bHasSpawned;
};


