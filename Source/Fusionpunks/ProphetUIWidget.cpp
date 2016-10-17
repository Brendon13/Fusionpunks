// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CyberHeroCharacter.h"
#include "AbilityBase.h"
#include "ProphetUIWidget.h"

FORCEINLINE FText UProphetUIWidget::GetArmySize() const
{
	return (owningCharacter ? FText::AsNumber(owningCharacter->GetArmySize()) : FText::AsNumber(0));
}

FORCEINLINE float UProphetUIWidget::GetAbility0CD() const
{
	if (owningCharacter)
	{
		return 1.0f;
	}
	return 0.0f;
}

FORCEINLINE float UProphetUIWidget::GetAbility1CD() const
{
	if (owningCharacter)
	{
		return 1.0f;
	}
	return 0.0f;
}

FORCEINLINE float UProphetUIWidget::GetAbility2CD() const
{
	if (owningCharacter)
	{
		return 1.0f;
	}
	return 0.0f;
}

FORCEINLINE float UProphetUIWidget::GetAbility3CD() const
{
	if (owningCharacter)
	{
		return 1.0f;
	}
	return 0.0f;
}

