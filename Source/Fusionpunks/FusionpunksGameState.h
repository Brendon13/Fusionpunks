// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "FusionpunksGameState.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API AFusionpunksGameState : public AGameState
{
	GENERATED_BODY()
public:

	AFusionpunksGameState();

	UFUNCTION(BlueprintCallable, Category = GameStatistics)
	void CyberLevelUp();

	UFUNCTION(BlueprintCallable, Category = GameStatistics)
		void DieselLevelUp();

	UFUNCTION(BlueprintCallable, Category = GameStatistics)
		int GetCyberLevel() const;

	UFUNCTION(BlueprintCallable, Category = GameStatistics)
		int GetDieselLevel() const;

private:
	int CyberLevel;
	int DieselLevel;
	
	
};
