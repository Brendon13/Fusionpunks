// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectOverTimeBase.h"
#include "LightningTowerDamage.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ALightningTowerDamage : public AEffectOverTimeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartTimer(float time, AActor* Target) override;

protected:
	UFUNCTION()
		virtual	void ApplyEffect() override;

	class AHeroBase* enemyHero = nullptr;
	class ACreep* enemyCreep = nullptr;
	class ACyberTower* towerOwner;
	

	
		
};
