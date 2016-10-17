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
	
	
};
