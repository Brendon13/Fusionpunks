// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CampProgressWidget.h"


#define LOCTEXT_NAMESPACE "MyNamespace"

FORCEINLINE float UCampProgressWidget::GetCyberCaptureProgress() const
{
	return (CurrentCamp != nullptr ? CurrentCamp->GetCyberCapturePercentage() : 0);
}

FORCEINLINE float UCampProgressWidget::GetDieselCaptureProgress() const
{
	return (CurrentCamp != nullptr ? CurrentCamp->GetDieselCapturePercentage() : 0);
}

FORCEINLINE FText UCampProgressWidget::GetCyberProgressText() const
{
	if (CurrentCamp != nullptr)
	{
		return FText::Format(LOCTEXT("CampProgress", "{0}%"), FText::AsNumber((int)(CurrentCamp->GetCyberCapturePercentage() * 100)));
	}
	return FText::AsNumber(0);
}

FORCEINLINE FText UCampProgressWidget::GetDieselProgressText() const
{
	if (CurrentCamp != nullptr)
	{
		return FText::Format(LOCTEXT("CampProgress", "{0}%"), FText::AsNumber((int)(CurrentCamp->GetDieselCapturePercentage() * 100)));
	}
	return FText::AsNumber(0);
}

FORCEINLINE void UCampProgressWidget::SetCurrentCreepCamp(ACreepCamp* newCreepCamp)
{
	CurrentCamp = newCreepCamp;
}

#undef LOCTEXT_NAMESPACE
