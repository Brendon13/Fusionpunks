// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "BulletBase.h"
#include "HeroBase.h"
#include "Ability_RangedAttack.h"



bool AAbility_RangedAttack::Ability()
{
	if (BulletClass)
	{
		//change to actors bullet spawn location
		AActor* owner = GetOwner();
		if (owner)
		{
			FActorSpawnParameters spawnParameters;
			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParameters.Owner = owner;

			GetWorld()->SpawnActor<ABulletBase>(BulletClass, owner->GetActorLocation() + (owner->GetActorForwardVector() * 100), FRotator::ZeroRotator, spawnParameters);
			return true;
		}
	}
	return false; 
}
