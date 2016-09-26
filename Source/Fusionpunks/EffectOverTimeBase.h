// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EffectOverTimeBase.generated.h"

UCLASS(Abstract)
class FUSIONPUNKS_API AEffectOverTimeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectOverTimeBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay
	(
		const EEndPlayReason::Type EndPlayReason
	) override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
		float effectTickTime;
	FTimerHandle applyEffectHandle;

	AActor* effectTarget;

public:
	
		virtual void StartTimer(float time, AActor* Target) { check(0 && "You must override this") };

		bool IsRunning();
		void StopTimer();
protected:
		UFUNCTION()
			virtual void ApplyEffect() { check(0 && "You must override this") };
	
	
};
