// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "PlayerHealthBarWidget.h"
#include "GameTimerWidget.h"
#include "CreepCamp.h"
#include "CampProgressWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHealthBarWidget> playerHealthBarWidgetClass;

	UPlayerHealthBarWidget* playerHealthBarWidget;

protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCampProgressWidget> campProgressWidgetClass;

	UCampProgressWidget* campProgressWidget;

public:
	UFUNCTION(BlueprintCallable, Category = PlayerUI)
		void ShowCampProgress(ACreepCamp* CurrentCamp);
	
	UFUNCTION(BlueprintCallable, Category = PlayerUI)
		void HideCampProgress();

protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGameTimerWidget> gameTimerWidgetClass;

	UGameTimerWidget* gameTimerWidget;
};
