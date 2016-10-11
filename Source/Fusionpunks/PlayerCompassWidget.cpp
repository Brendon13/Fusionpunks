// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "PlayerCompassWidget.h"


void UPlayerCompassWidget::NativeConstruct()
{
	Super::NativeConstruct();

	imageAlpha = 1.0f;
	alphaMultiplier = 0.4f;
	bCountUp = false; 
}

void UPlayerCompassWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (owningHero != nullptr)
	{
		if (owningHero->GetPlayerHealthAsDecimal() <= 0.15f)
		{
			if (PulseAnimation && !IsPlayingAnimation())
			{
				PlayAnimation(PulseAnimation);
			}
		}
		else
		{
			StopAnimation(PulseAnimation);
		}
	}
}


FLinearColor UPlayerCompassWidget::SetCompassColor() const
{
	if (owningHero != nullptr)
	{
		float red = (1 - owningHero->GetPlayerHealthAsDecimal());
		float green = owningHero->GetPlayerHealthAsDecimal();
		float blue = 0;
		return FLinearColor::FLinearColor(red, green, blue, imageAlpha);
	}
	UE_LOG(LogTemp, Warning, TEXT("Hero reference in PlayerCompassWidget is NULL!"));

	return FLinearColor::White;
}