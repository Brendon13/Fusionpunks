// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"

#include "HeroBase.h"
#include "CyberHeroCharacter.generated.h"

UCLASS(config=Game)
class ACyberHeroCharacter: public AHeroBase
{
	GENERATED_BODY()

public:
	ACyberHeroCharacter();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);
	

private:
	UPROPERTY(EditAnywhere, category = Ability)
		TSubclassOf<class AAbilityBase> chainLightningAbility;
	
	TArray<AActor*> affectedActors;
	bool skillSelected = false;


public:
	void DetermineClickEvent();
	void OnSkillPressed();
	void UseSkill(AActor* enemy);
	
	void AddAffectedActor(AActor* enemy);
	bool IsAffected(AActor* enemy);
	void HighlightTarget(AActor* enemy, TArray<FOverlapResult> enemies);
	void UnHighlightTarget(AActor* enemy);
	void UnHighlightAll(TArray<FOverlapResult> enemies);
	AActor* UpdateTarget();
	

//Skill Target related
private:
	TArray<FOverlapResult> skillTargetResults;
	TArray<FOverlapResult> oldTargetResults;

	
};

