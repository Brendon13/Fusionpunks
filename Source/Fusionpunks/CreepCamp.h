// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CreepCamp.generated.h"

UENUM(BlueprintType)
enum class ECampType : uint8
{
	CT_Neutral    UMETA(DisplayName = "Neutral"),
	CT_Cyber      UMETA(DisplayName = "Cyber"),
	CT_Diesel	  UMETA(DisplayName = "Diesel")
};

UCLASS()
class FUSIONPUNKS_API ACreepCamp : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Stats)
		ECampType campType;
	
public:	
	ACreepCamp();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		 TSubclassOf<class ANeutralCreep> neutralCreepRef;

	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		TSubclassOf<class ACyberCreep> cyberCreepRef;

	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		TSubclassOf<class ADieselCreep> dieselCreepRef;

//Meshes and triggers 
protected:
	UPROPERTY(EditDefaultsonly, Category = Appearance)
		UStaticMeshComponent* campMesh; 

	UPROPERTY(EditDefaultsonly, Category = CollisionComponents)
		USphereComponent* sphereTrigger;

	//mesh for the ring around the camp
	UPROPERTY(EditDefaultsonly, Category = Appearance)
		UStaticMeshComponent* ringMesh;


//Ring Rotations Stuff
protected:
	UPROPERTY(EditAnywhere, Category = CampVariables)
	float ringRotationSpeed;

	UPROPERTY(EditAnywhere, Category = CampVariables)
	float ringMaterialAlphaSpeed;

	FRotator ringRotation;
	float ringMaterialAlpha;
	bool bCountUp;


//Overlap functions
protected:
	//function for Trigger Events
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//function for Trigger Exit Events
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


//Capture Camp stuff
public:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetCyberCapturePercentage();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetDieselCapturePercentage();
protected:
	float cyberCaptureProgress;
	float dieselCaptureProgress;

	bool bCyberIsCapturing;
	bool bDieselIsCapturing;

	//The more creeps that are in the camp the higher the capture time should be 
	//When a camp is captured all the creeps associated with it should die 
	float captureTime;
	int creepCount; //Spawn rate for creeps is based on how many are currently at the camp... Less = faster spawn rate, More = slower spawn rate

public:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void MinusOneFromCreepCamp();


//Creep Spawn Locations
protected:
	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep1SpawnLocation;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep2SpawnLocation;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep3SpawnLocation;

	TArray<FVector> creepSpawnArray;


protected:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToDieselCamp();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToCyberCamp();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToNeutralCamp();
};
