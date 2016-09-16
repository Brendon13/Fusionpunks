// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "PlayerHealthBarWidget.h"

#define LOCTEXT_NAMESPACE "MyNamespace"

float UPlayerHealthBarWidget::GetPlayerHealthPercentage() const
{
	AHeroBase* MyCharacter = Cast<AHeroBase>(GetOwningPlayerPawn());

	return MyCharacter->GetCurrentHealth() / MyCharacter->GetMaxHealth() * 100;
}

FText UPlayerHealthBarWidget::GetPlayerHealthText() const
{
	AHeroBase* MyCharacter = Cast<AHeroBase>(GetOwningPlayerPawn());

	return FText::Format(LOCTEXT("HealthFormat", "Health: {0} / {1}"),
		FText::AsNumber(MyCharacter->GetCurrentHealth()),
		FText::AsNumber(MyCharacter->GetMaxHealth()));
}

#undef LOCTEXT_NAMESPACE