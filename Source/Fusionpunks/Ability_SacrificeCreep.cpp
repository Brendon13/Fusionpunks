// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "Ability_SacrificeCreep.h"



bool AAbility_SacrificeCreep::Ability()
{
	AHeroBase* hero = Cast<AHeroBase>(GetOwner());
	if (hero)
	{
		if(hero->SacrificeCreep())
			return true; 
	}
	return false; 
}