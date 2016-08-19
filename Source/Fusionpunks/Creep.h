// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Creep.generated.h"

UCLASS()
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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float maxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		int maxLevel;

	UPROPERTY(EditAnywhere, Category = "Stats")
		float movementSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float damage;

private:
	float currentHealth;
	int currentLevel;

protected:
	UPROPERTY(EditAnywhere, Category = Appearance)
	UStaticMeshComponent* creepMesh;
	
	
};
