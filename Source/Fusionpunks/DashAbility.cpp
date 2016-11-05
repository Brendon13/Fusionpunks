// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "DashAbility.h"


void ADashAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DashTimer -= DeltaSeconds;

	if (DashTimer <= 0 && bHasDashed)
	{
		AHeroBase* hero = Cast<AHeroBase>(GetOwner());
		if (hero)
		{
			FVector opposingForce = hero->GetCharacterMovement()->Velocity;
			hero->GetCharacterMovement()->AddForce(-opposingForce);
			hero->GetCharacterMovement()->UpdateComponentVelocity();
			//hero->GetCharacterMovement()->AddForce()
			hero->GetCharacterMovement()->GroundFriction = 8.0f;
			bHasDashed = false; 
		}
	}
}

bool ADashAbility::Ability()
{
	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		bHasDashed = true; 
		DashTimer = TargetDashTime;
		hero->GetCharacterMovement()->GroundFriction = 0.0f;
		hero->GetCharacterMovement()->AddImpulse(hero->GetActorForwardVector() * DashForce, false);
		//hero->GetCharacterMovement()->AddForce(hero->GetActorUpVector() * -DashForce);
		hero->GetCharacterMovement()->UpdateComponentVelocity();
	
		return true;
	}
	return false; 
}