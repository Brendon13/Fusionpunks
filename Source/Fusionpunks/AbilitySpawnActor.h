// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "AbilitySpawnActor.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AAbilitySpawnActor : public AAbilityBase
{
	GENERATED_BODY()

public:
	virtual void Use() override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> spawnClass;
	
	
};
