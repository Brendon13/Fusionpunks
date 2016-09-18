// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Projectile.h"
#include "HealthBarWidgetComponent.h"

#include "BaseHealthBar.h"
#include <Classes/Kismet/KismetMathLibrary.h>
#include "GameFramework/Actor.h"
#include <Components/LineBatchComponent.h>
#include "Tower.generated.h"

// OLD TOWER CLASS... NOT USING ANYMORE!!!!!!
// OLD TOWER CLASS... NOT USING ANYMORE!!!!!!
// OLD TOWER CLASS... NOT USING ANYMORE!!!!!!

UCLASS()
class FUSIONPUNKS_API ATower : public AActor
{
	GENERATED_BODY()
	

	

public:	
	// Sets default values for this actor's properties
	ATower();

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	float GetHpPercent();

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	FVector myLocation;

	float spawnTimer;

	void SpawnProjectile();

	bool bHasSpawed = false;

	UPROPERTY(EditDefaultsOnly)
		UHealthBarWidgetComponent *healthBar;

	UPROPERTY(EditDefaultsOnly)
		UShapeComponent* radius;

	UPROPERTY(EditDefaultsOnly)
		UShapeComponent* rootCldr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* visualTower;

	UFUNCTION()
		void TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		TSubclassOf<class AProjectile> whatToSpawn;

	AProjectile* currProjectile;

	bool bIsEnemyUnit = false;
	
	TArray<AActor*> enemyUnits;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
		bool bIsCannonTower = true;
	
	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* beam;
	
	UPROPERTY(EditDefaultsOnly)
		float maxHP = 100;

		float currHP;
		
		APlayerController* playerCam;

		AActor* playerChar;
};
