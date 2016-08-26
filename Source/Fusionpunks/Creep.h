// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Creep.generated.h"

class ACreepCamp;

UCLASS(abstract)
class FUSIONPUNKS_API ACreep : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACreep();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Stats")
		const float GetHealthAsDecimal() const
	{
		return currentHealth / maxHealth;
	}

	UFUNCTION(BlueprintCallable, category = "Stats")
	void LevelUp()
	{
		if (currentLevel + 1 <= maxLevel)
		{
			currentLevel++;
			maxHealth += healthIncreasePerLevel;
			damage += damageIncreasePerLevel;
		}
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float maxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		int maxLevel;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float movementSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float damage;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float healthIncreasePerLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float damageIncreasePerLevel;

private:
	float currentHealth;
	int currentLevel;

protected:
	UPROPERTY(EditAnywhere, Category = Appearance)
	UStaticMeshComponent* creepMesh;
	

public:
	//function to set home creep camp
	void SetCreepCampHome(ACreepCamp* home)
	{
		creepCampHome = home;
	}

private:
	ACreepCamp* creepCampHome;
	
};
