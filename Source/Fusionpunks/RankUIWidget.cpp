// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "RankUIWidget.h"


FORCEINLINE FText URankUIWidget::GetArmySize() const
{
	return (owningCharacter ? FText::AsNumber(owningCharacter->GetArmySize()) : FText::AsNumber(0));
}

