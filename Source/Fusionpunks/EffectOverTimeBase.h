// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EffectOverTimeBase.generated.h"

UCLASS()
class FUSIONPUNKS_API AEffectOverTimeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectOverTimeBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
protected:
	UPROPERTY(EditDefaultsOnly)
		float effectTickTime;
	FTimerHandle applyEffectHandle;

protected:
	virtual void ApplyEffect();
	
};
