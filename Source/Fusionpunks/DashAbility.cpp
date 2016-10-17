// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "DashAbility.h"




void ADashAbility::Use()
{
	Super::Use();

	if (CanUse())
	{
		Dash();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dash Ability on Cooldown!"));
	}
	
}

void ADashAbility::Dash()
{
	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		hero->GetCapsuleComponent()->AddImpulse(hero->GetActorForwardVector() * DashForce);
	}
}