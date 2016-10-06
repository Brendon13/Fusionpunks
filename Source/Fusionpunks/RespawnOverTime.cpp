// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "RespawnOverTime.h"




void ARespawnOverTime::StartTimer(float time, AActor* target)
{

	GetWorld()->GetTimerManager().SetTimer(applyEffectHandle, this, &ARespawnOverTime::ApplyEffect, time, false);
	effectTarget = target;
}

void ARespawnOverTime::ApplyEffect()
{
	AHeroBase* hero = Cast<AHeroBase>(effectTarget);
	hero->SetActorLocation(hero->startingLocation);
	hero->GetMesh()->SetVisibility(true);
	hero->SetActorEnableCollision(true);
	hero->GetController()->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	hero->ResetHealth();
	hero->bIsRespawning = false;
}