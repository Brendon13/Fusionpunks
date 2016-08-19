// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameTimerWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UGameTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = GameVariables)
		FText GetGameTimeAsText() const;

	
private:
	UPROPERTY(EditAnywhere, Category = GameVariables)
	float GameLength;

	float gameTimer;
	
	
};
