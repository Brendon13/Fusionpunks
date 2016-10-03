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
	
	class AProjectile* SpawnProjectile();
private:
	

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class AProjectile> whatToSpawn;

	AProjectile* currProjectile;
	class AProjectileTowerDamage* towerDMG;
	bool bHasSpawned;

protected:
	virtual void CleanUp() override;

	UFUNCTION()
		virtual void TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};


