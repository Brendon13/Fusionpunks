// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "TowerDamage.h"

void ATowerDamage::ApplyEffect(float time, AActor* target) 
{
	
	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &ATowerDamage::ApplyDamage, time, true);
	attackTarget = target;
}
void ATowerDamage::ApplyDamage()
{
	UE_LOG(LogTemp, Log, TEXT("TOWER DAMAGE!"));
	FDamageEvent DamageEvent;

	float damage = attackTarget->TakeDamage(2.0f, DamageEvent,NULL,this);
}

