// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectOverTimeBase.h"
#include "ProjectileTowerDamage.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AProjectileTowerDamage : public AEffectOverTimeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartTimer(float time, AActor* Target) override;
	void PauseTimer();
	bool IsPaused();
	void UnPauseTimer(AActor* newTarget);
protected:	
	UFUNCTION()
		virtual	void ApplyEffect() override;
	

	
	
};
