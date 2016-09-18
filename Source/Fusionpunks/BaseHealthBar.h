// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "HealthBarWidgetComponent.h"

#include "BaseHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UBaseHealthBar : public UUserWidget
{
	GENERATED_BODY()


protected:
	AActor* owningActor;

public:

	UFUNCTION(BlueprintCallable, category = health)
		float GetHpPercent() const;
	
	void SetOwningActor(AActor* owner);

	AActor* GetOwningActor();

	
};
