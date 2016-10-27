// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CyberHeroCharacter.h"
#include "AbilityBase.h"
#include "ProphetUIWidget.h"

FText UProphetUIWidget::GetArmySize() const
{
	return (owningCharacter ? FText::AsNumber(owningCharacter->GetArmySize()) : FText::AsNumber(0));
}

FText UProphetUIWidget::GetCurrentLevel() const
{
	return (owningCharacter ? FText::AsNumber(owningCharacter->GetLevel()) : FText::AsNumber(0));
}

float UProphetUIWidget::GetCurrentExperience() const
{
	return (owningCharacter ? owningCharacter->GetCurrentExperienceAsDecimal() : 0);
}

float UProphetUIWidget::GetAbility0CD() const
{
	if (owningCharacter)
	{
		AAbilityBase* ability = owningCharacter->GetAbility(0);
		if (ability)
		{
			return 1 - ability->GetCoolDownRemainingAsDecimal();
		}
		return 1.0f;
	}
	return 0.0f;
}

float UProphetUIWidget::GetAbility1CD() const
{
	if (owningCharacter)
	{
		AAbilityBase* ability = owningCharacter->GetAbility(1);
		if (ability)
		{
			return 1 - ability->GetCoolDownRemainingAsDecimal();
		}
		return 1.0f;
	}
	return 0.0f;
}

float UProphetUIWidget::GetAbility2CD() const
{
	if (owningCharacter)
	{
		AAbilityBase* ability = owningCharacter->GetAbility(2);
		if (ability)
		{
			return 1 - ability->GetCoolDownRemainingAsDecimal();
		}
		return 1.0f;
	}
	return 0.0f;
}

float UProphetUIWidget::GetAbility3CD() const
{
	if (owningCharacter)
	{
		AAbilityBase* ability = owningCharacter->GetAbility(3);
		if (ability)
		{
			return 1 - ability->GetCoolDownRemainingAsDecimal();
		}
		return 1.0f;
	}
	return 0.0f;
}

float UProphetUIWidget::GetAbility4CD() const
{
	if (owningCharacter)
	{
		AAbilityBase* ability = owningCharacter->GetAbility(4);
		if (ability)
		{
			return 1 - ability->GetCoolDownRemainingAsDecimal();
		}
		return 1.0f;
	}
	return 0.0f;
}

