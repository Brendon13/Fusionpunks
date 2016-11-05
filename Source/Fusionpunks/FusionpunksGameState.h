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
		FORCEINLINE	int GetCyberLevel() const { return CyberLevel; }

	UFUNCTION(BlueprintCallable, Category = GameStatistics)
		FORCEINLINE int GetDieselLevel() const { return DieselLevel; }

	TArray<class AHeroBase*> Players;

	void DieselWins();
	void CyberWins();

private:
	int CyberLevel;
	int DieselLevel;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
		TSubclassOf<class UUserWidget> CyberWinsWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
		TSubclassOf<class UUserWidget> DieselWinsWidgetClass;



	

	
};
