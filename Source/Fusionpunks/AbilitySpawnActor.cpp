// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AbilitySpawnActor.h"

void AAbilitySpawnActor:: Use() {
	Super::Use();

	GetWorld()->SpawnActor <AActor>(spawnClass, GetActorLocation(), FRotator::ZeroRotator);
}


