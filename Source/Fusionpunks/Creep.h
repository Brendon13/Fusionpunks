// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CreepHealthbarWidget.h"
#include "WidgetComponent.h"
#include "Creep.generated.h"

class ACreepCamp;

UCLASS(abstract)
class FUSIONPUNKS_API ACreep : public APawn
{
	GENERATED_BODY()

public:
	ACreep();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Stats)
		const float GetHealthAsDecimal() const  { return currentHealth / maxHealth; }

	UFUNCTION(BlueprintCallable, category = Stats)
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

protected:
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		USkeletalMeshComponent* creepSkeletalMeshComp;


//Creep Camp stuff
public:
	void SetCreepCampHome(ACreepCamp* home, bool BelongsToCamp = true);
	bool bBelongsToCamp; //false means the creep belongs to a player (i.e. is apart of a player army)
protected:
	ACreepCamp* creepCampHome;
	

//health bar widget stuff
protected:
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	UWidgetComponent* widgetComponent;

	TSubclassOf<UCreepHealthbarWidget> CreepHealthBarWidgetClass;

	FRotator widgetCompRotation;
	AActor* localPlayer;

//Damage stuff
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass; 
	//UWidgetComponent* floatingDamageWidget;
};
