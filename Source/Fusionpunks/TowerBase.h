// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.h"
#include <Classes/Kismet/KismetMathLibrary.h>
#include "TowerBase.generated.h"

UCLASS(Abstract)
class FUSIONPUNKS_API ATowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATowerBase();
	
public:
	// Sets default values for this actor's properties
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	float GetHpPercent();

protected:

	FVector myLocation;

	float spawnTimer;


	bool bHasSpawed = false;

	//UPROPERTY(EditDefaultsOnly)
	//	UHealthBarWidgetComponent *healthBar;

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


	TArray<AActor*> enemyUnits;

	UPROPERTY(EditDefaultsOnly)
		float maxHP = 100;

	float currHP;

	//APlayerController* playerCam;

	//AActor* playerChar;

	
	
};
