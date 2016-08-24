// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TowerBase.h"
#include "CyberTower.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ACyberTower : public ATowerBase
{
	GENERATED_BODY()
	
public:

	ACyberTower();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

private:
	
	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* beam;

	FVector sourceLocation;
};