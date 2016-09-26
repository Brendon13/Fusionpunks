// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CyberTower.h"
#include "LightningTowerDamage.h"

void ALightningTowerDamage::StartTimer(float time, AActor* target) 
{
	
	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &ALightningTowerDamage::ApplyEffect, time, true);
	effectTarget = target;
}
void ALightningTowerDamage::ApplyEffect()
{
	UE_LOG(LogTemp, Log, TEXT("TOWER DAMAGE!"));
	FDamageEvent DamageEvent;
	float damage = effectTarget->TakeDamage(Cast<ACyberTower>(GetOwner())->damage, DamageEvent,NULL,this);
}

