// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CyberTower.h"
#include "HeroBase.h"
#include "Creep.h"
#include "LightningTowerDamage.h"

void ALightningTowerDamage::StartTimer(float time, AActor* target) 
{
	
	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &ALightningTowerDamage::ApplyEffect, time, true);
	effectTarget = target;
	enemyHero = nullptr;
	enemyCreep = nullptr;

	if (towerOwner == nullptr)
	{
		towerOwner = Cast<ACyberTower>(GetOwner());
	}

	if (effectTarget != nullptr && effectTarget->IsA(AHeroBase::StaticClass()))
	{		
		enemyHero = Cast<AHeroBase>(effectTarget);		
	}
	else if (effectTarget != nullptr && effectTarget->IsA(ACreep::StaticClass()))
	{
		enemyCreep = Cast<ACreep>(effectTarget);
	}



}
void ALightningTowerDamage::ApplyEffect()
{
	UE_LOG(LogTemp, Log, TEXT("TOWER DAMAGE!"));
	FDamageEvent DamageEvent;

	if (effectTarget->IsA(AHeroBase::StaticClass()) && enemyHero != nullptr)
	{
		if (enemyHero->GetPlayerHealthAsDecimal() <= 0 || enemyHero->IsRespawning())
		{
			StopTimer();
			towerOwner->RemoveFromTargetList(effectTarget);
			towerOwner->SetIsDealingDamage(false);
			
			
		}
		else
		{
			float damage = effectTarget->TakeDamage(towerOwner->damage, DamageEvent, NULL, towerOwner);
		}
	}
	else if (effectTarget->IsA(ACreep::StaticClass()) && enemyCreep!= nullptr)
	{
		if (enemyCreep->GetBIsDead())
		{
			StopTimer();
			towerOwner->RemoveFromTargetList(effectTarget);
			towerOwner->SetIsDealingDamage(false);			
		}
		else
		{
			float damage = effectTarget->TakeDamage(towerOwner->damage, DamageEvent, NULL, towerOwner);
		}
	}
	
}

