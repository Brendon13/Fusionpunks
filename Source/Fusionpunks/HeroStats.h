// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class FUSIONPUNKS_API HeroStats
{
public:
	HeroStats(class AHeroBase* Hero);
	~HeroStats();
	void UpdateStats();
	void DisplayStats();

private:
	class AHeroBase* hero;
	float currentHealth, respawnTime, attackDamage;
	int32 level, armySize, numCampsCaptured;
	TArray<class ACreepCamp*> capturedCamps;

	FName team;
	




};
