// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "RankUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API URankUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	class ADieselHeroCharacter* owningCharacter;

public:
	UFUNCTION(BlueprintCallable, Category = Player)
		FText GetArmySize() const;

	FORCEINLINE void SetOwningCharacter(ADieselHeroCharacter* dieselCharacter) { owningCharacter = dieselCharacter; }

	UFUNCTION(BlueprintCallable, Category = Player)
		FText GetCurrentLevel() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		float GetCurrentExperience() const;

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility0CD() const;

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility1CD() const;

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility2CD() const;

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility3CD() const;
};
