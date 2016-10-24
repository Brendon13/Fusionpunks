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
	
	// Called every fram)e
	virtual void Tick( float DeltaSeconds ) override;

	virtual void Use();

	virtual bool CanUse();

	FORCEINLINE float GetCoolDownRemainingAsDecimal() const { return (coolDownLeft >= 0 ? coolDownLeft / maxCoolDown : 0.0f); }
	FORCEINLINE float GetCoolDownRemaining() const { return (coolDownLeft >= 0 ? coolDownLeft : 0.0f); }

protected:
	UPROPERTY(EditDefaultsonly, Category = Cooldown)
	float maxCoolDown;

	//if ability is successful return true to start cooldown 
	virtual bool Ability() { check(0 && "You must override this") return true; }

private:
	float coolDownLeft;

	
	
};
