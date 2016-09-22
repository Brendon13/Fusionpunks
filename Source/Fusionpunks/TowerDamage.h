// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectOverTimeBase.h"
#include "TowerDamage.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ATowerDamage : public AEffectOverTimeBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(float time, AActor* Target) override;

private:

	UFUNCTION()
		void ApplyDamage();

	AActor* attackTarget;
		
};
