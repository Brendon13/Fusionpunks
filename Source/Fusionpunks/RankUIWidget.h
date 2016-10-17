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
	
};
