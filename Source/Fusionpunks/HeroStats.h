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
	float currentHealth, respawnTime, attackDamage, healthPercent;
	int32 level, armySize, numCampsCaptured;
	TArray<class ACreepCamp*> capturedCamps;
	FName team;

public:
	FORCEINLINE float GetHealthPercent() const { return healthPercent; }
	FORCEINLINE int32 GetArmySize() const { return armySize; }


};
