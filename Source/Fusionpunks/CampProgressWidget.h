// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CreepCamp.h"
#include "CampProgressWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UCampProgressWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetCyberCaptureProgress() const;

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetDieselCaptureProgress() const;

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		FText GetCyberProgressText() const;

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		FText GetDieselProgressText() const;

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetCurrentCreepCamp(ACreepCamp* newCreepCamp);


	
protected:
	ACreepCamp* CurrentCamp;
};
