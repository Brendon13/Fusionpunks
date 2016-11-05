// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "DashAbility.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API ADashAbility : public AAbilityBase
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual bool Ability() override;

	UPROPERTY(EditDefaultsOnly, Category = DashAbility)
		float DashForce;

	UPROPERTY(EditDefaultsOnly, Category = DashAbility)
		float TargetDashTime;

	float DashTimer;
	bool bHasDashed;
};
