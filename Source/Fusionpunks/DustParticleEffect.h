// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DustParticleEffect.generated.h"

UCLASS()
class FUSIONPUNKS_API ADustParticleEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADustParticleEffect();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Particle System")
		UParticleSystemComponent* dustParticleEffect;
	
};
