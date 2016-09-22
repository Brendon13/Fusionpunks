// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "EffectOverTimeBase.h"


// Sets default values
AEffectOverTimeBase::AEffectOverTimeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEffectOverTimeBase::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AEffectOverTimeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (applyEffectHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(applyEffectHandle);


}

bool AEffectOverTimeBase::IsRunning() 
{

	return applyEffectHandle.IsValid();

}

void AEffectOverTimeBase::StopTimer() 
{
	if (applyEffectHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(applyEffectHandle);

}




