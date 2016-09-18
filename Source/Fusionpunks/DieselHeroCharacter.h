// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "CreepCamp.h"
#include "DustParticleEffect.h"
#include "HeroBase.h"
#include "DieselHeroCharacter.generated.h"

UCLASS(config=Game)
class ADieselHeroCharacter : public AHeroBase
{

public:
	GENERATED_BODY()

	ADieselHeroCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
//Variables and functions for Leap Ability 
protected:
	UFUNCTION(BlueprintCallable, Category = Abilities)
		void Leap();

	UPROPERTY(EditAnywhere, Category = LeapAbility)
		float leapUpwardForce;

	UPROPERTY(EditAnywhere, Category = LeapAbility)
		float leapDownwardForce;

	UPROPERTY(EditAnywhere, Category = LeapAbility)
		float leapMaxAirTime;
	UFUNCTION(BlueprintCallable, Category = Attack)
		void ForcePull();
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	float timeInAirTimer;
	bool bIsLeaping;

//Pull Force Ability stuff
protected:
	

	UPROPERTY(EditAnywhere, Category = PullAttack)
		float pullRadius;

	UPROPERTY(EditAnywhere, Category = PullAttack)
		float pullForce;

	

//Dash Ability Stuff
private:
	UFUNCTION(BlueprintCallable, Category = Abilities)
		void Dash();

	UPROPERTY(EditAnywhere, Category = DashAbility)
		float dashForce;

	UPROPERTY(EditAnywhere, Category = DashAbility)
		float dashMaxTime;

	UPROPERTY(EditAnywhere, Category = "AbilityEffects")
		TSubclassOf<AActor> dustParticleSystemClass;
private:
	float dashTimer;
	bool bIsDashing;




};

