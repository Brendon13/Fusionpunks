// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "PlayerHealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UPlayerHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Player)
		float GetPlayerHealthAsDecimal() const;

	UFUNCTION(BlueprintCallable, Category = Player)
		FText GetPlayerHealthText() const;	
};
