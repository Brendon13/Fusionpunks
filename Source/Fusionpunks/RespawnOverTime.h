// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectOverTimeBase.h"
#include "RespawnOverTime.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ARespawnOverTime : public AEffectOverTimeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartTimer(float time, AActor* Target) override;

protected:
	UFUNCTION()
		virtual	void ApplyEffect() override;
	
	
};
