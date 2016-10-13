// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectOverTimeBase.h"
#include "HealOverTime.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AHealOverTime : public AEffectOverTimeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartTimer(float time, AActor* Target) override;
	void SetTotalHealthValue(float healthValue, float tickTime);

protected:
	UFUNCTION()
		virtual	void ApplyEffect() override;

	float totalHealthValue;
	float amountHealed;
	float tickHealAmount;

	
	
	
};
