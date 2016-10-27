// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_SacrificeCreep.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_SacrificeCreep : public AAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Ability() override;
	
};
