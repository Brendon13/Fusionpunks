// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "Ability_ForcePush.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbility_ForcePush : public AAbilityBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	virtual bool Ability() override;

	UPROPERTY(EditDefaultsOnly, Category = ForcePushAbility)
		float PushRadius;

	UPROPERTY(EditDefaultsOnly, Category = ForcePushAbility)
		float PushForce;

	UPROPERTY(EditDefaultsOnly, Category = ForcePushAbility)
		float damage; 


};
