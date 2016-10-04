// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CreepHealthbarWidget.h"
#include "WidgetComponent.h"
#include "CreepAIController.h"
#include "Creep.generated.h"

class ACreepCamp;

UCLASS(abstract)
class FUSIONPUNKS_API ACreep : public ACharacter
{
	GENERATED_BODY()

public:
	ACreep();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Stats)
		const float GetHealthAsDecimal() const  { return currentHealth / maxHealth; }

	UFUNCTION(BlueprintCallable, Category = Stats)
		void LevelUp();

//editable stats 
protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float maxHealth;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int maxLevel;

	UPROPERTY(EditAnywhere, Category = Stats)
		float movementSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float damage;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float healthIncreasePerLevel;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float damageIncreasePerLevel;

	float currentHealth;
	int currentLevel;
public:
	float GetDamage() const { return damage; }

//Creep Camp stuff
public:
	void SetCreepCampHome(ACreepCamp* home, bool BelongsToCamp);
	bool bBelongsToCamp; //false means the creep belongs to a player (i.e. is apart of a player army)
protected:
	ACreepCamp* creepCampHome;
	

//health bar widget stuff
protected:
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* widgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<UCreepHealthbarWidget> CreepHealthBarWidgetClass;

	FRotator widgetCompRotation;
	AActor* localPlayer;


//Damage stuff
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass; 


//AISTUFF
	UPROPERTY(EditDefaultsOnly)
		ACreepAIController* AiController; 

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float patrolRadius;
public:
	UFUNCTION(BlueprintCallable, Category = CampVariables)
		float GetPatrolRadius();

	UFUNCTION(BlueprintCallable, Category = CampVariables)
		ACreepCamp* GetCreepCampHome() const; 


//stuff for Character Movement
protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);
	/** Called for side to side input */
	void MoveRight(float Value);
	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);
	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

};
