// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "HeroBase.h"
#include "HeroStats.h"

HeroStats::HeroStats(AHeroBase* Hero)
{
	hero = Hero;
	currentHealth = hero->GetCurrentHealth();
	healthPercent = hero->GetPlayerHealthAsDecimal();
	respawnTime = hero->GetRespawnTime();
	attackDamage = hero->GetAttackDamage();
	level = hero->GetLevel();
	armySize = hero->GetArmySize();
	team = hero->GetTeam();
	capturedCamps = hero->GetCapturedCamps();
	numCampsCaptured = capturedCamps.Num();
	
}

HeroStats::~HeroStats()
{
}

void HeroStats::UpdateStats()
{
	currentHealth = hero->GetCurrentHealth();
	healthPercent = hero->GetPlayerHealthAsDecimal();
	respawnTime = hero->GetRespawnTime();
	attackDamage = hero->GetAttackDamage();
	level = hero->GetLevel();
	armySize = hero->GetArmySize();
	team = hero->GetTeam();
	capturedCamps = hero->GetCapturedCamps();
	numCampsCaptured = capturedCamps.Num();
}

void HeroStats::DisplayStats()
{
	UE_LOG(LogTemp, Warning, TEXT("Hero Type: %s"), *team.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Respawn Time: %f"), respawnTime);
	UE_LOG(LogTemp, Warning, TEXT("Attack Damage: %f"), attackDamage);
	UE_LOG(LogTemp, Warning, TEXT("Level: %d"), level);
	UE_LOG(LogTemp, Warning, TEXT("ArmySize: %d"), armySize);
	UE_LOG(LogTemp, Warning, TEXT("Number of captured camps: %d"), numCampsCaptured)
}