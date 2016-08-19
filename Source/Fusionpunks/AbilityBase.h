// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"

UCLASS(Abstract)
class FUSIONPUNKS_API AAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Use();

	virtual bool CanUse();

protected:
	UPROPERTY(EDITDefaultsOnly)
		float maxCoolDown;

private:
	float coolDownLeft;

	
	
};
