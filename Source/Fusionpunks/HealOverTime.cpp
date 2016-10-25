// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "HealOverTime.h"

void AHealOverTime::StartTimer(float time, AActor* target)
{
	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &AHealOverTime::ApplyEffect, time, true);
	effectTarget = target;
	amountHealed = 0;
}
void AHealOverTime::ApplyEffect()
{
	//UE_LOG(LogTemp, Log, TEXT("Healing Hero!"));
	
	AHeroBase* heroBase = Cast<AHeroBase>(effectTarget);
	
	
	if (GetOwner()->IsA(AHeroBase::StaticClass()))
	{
		if (heroBase)
		{
			heroBase->Heal(tickHealAmount);
		}

		amountHealed += tickHealAmount;
		if (amountHealed > totalHealthValue)
		{
			StopTimer();
			Destroy();
		}
	}

	else 
	{
		if (heroBase->GetCurrentHealth() < heroBase->GetMaxHealth() )
		{
			if (heroBase)
			{
				heroBase->Heal(tickHealAmount);
			}
		}

		else
		{
			heroBase->ResetHealth();
			StopTimer();
		}
	}

	//call function to heal hero
	//check if we are at max
	//if we are clear time destory actor
}

void AHealOverTime::SetTotalHealthValue(float healthValue, float tickTime)
{
	totalHealthValue = healthValue; 
	tickHealAmount = totalHealthValue * tickTime;
}

