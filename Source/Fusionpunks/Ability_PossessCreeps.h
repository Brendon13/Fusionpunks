// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_PossessCreeps.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_PossessCreeps : public AAbilityBase
{
	GENERATED_BODY()

public:
	//if ability is successful return true to start cooldown
	virtual bool Ability() override;
	
};
