// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_RangedAttack : public AAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Ability() override;
	
	UPROPERTY(EditDefaultsOnly, Category = BulletType)
		TSubclassOf<class ABulletBase> BulletClass;
};
