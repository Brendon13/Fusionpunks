// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CampProgressWidget.h"


#define LOCTEXT_NAMESPACE "MyNamespace"

float UCampProgressWidget::GetCyberCaptureProgress() const
{
	if (CurrentCamp != nullptr)
	{
		return CurrentCamp->GetCyberCapturePercentage();
	}
	else
	{
		return 0;
	}
}

float UCampProgressWidget::GetDieselCaptureProgress() const
{
	if (CurrentCamp != nullptr)
	{
		return CurrentCamp->GetDieselCapturePercentage();
	}
	else
	{
		return 0;
	}
}

FText UCampProgressWidget::GetCyberProgressText() const
{
	if (CurrentCamp != nullptr)
	{
		return FText::Format(LOCTEXT("CampProgress", "{0}%"), FText::AsNumber((int)(CurrentCamp->GetCyberCapturePercentage() * 100)));
	}
	else
	{
		return FText::AsNumber(0);
	}
}

FText UCampProgressWidget::GetDieselProgressText() const
{
	if (CurrentCamp != nullptr)
	{
		return FText::Format(LOCTEXT("CampProgress", "{0}%"), FText::AsNumber((int)(CurrentCamp->GetDieselCapturePercentage() * 100)));
	}
	else
	{
		return FText::AsNumber(0);
	}
}

void UCampProgressWidget::SetCurrentCreepCamp(ACreepCamp* newCreepCamp)
{
	CurrentCamp = newCreepCamp;
}

#undef LOCTEXT_NAMESPACE
