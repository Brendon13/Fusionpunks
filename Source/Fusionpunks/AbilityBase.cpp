// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "AbilityBase.h"

// Stuff
// Sets default values
AAbilityBase::AAbilityBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	coolDownLeft = 0.0f;
	maxCoolDown = 1.5f;

}

// Called when the game starts or when spawned
void AAbilityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbilityBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	coolDownLeft -= DeltaTime;
	
}
void AAbilityBase::Use()
{
	if (CanUse())
	{
		coolDownLeft = maxCoolDown;
	}
}
bool AAbilityBase::CanUse()
{
	return coolDownLeft <= 0;
}


