// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ProphetUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UProphetUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	class ACyberHeroCharacter* owningCharacter;
	
public:
	UFUNCTION(BlueprintCallable, Category = Player)
	FText GetArmySize() const;

	FORCEINLINE void SetOwningCharacter(ACyberHeroCharacter* cyberCharacter) { owningCharacter = cyberCharacter; }

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility0CD() const;

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility1CD() const;

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility2CD() const;

	UFUNCTION(BlueprintCallable, Category = WidgetFunctions)
		float GetAbility3CD() const;
	
};
