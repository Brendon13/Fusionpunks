// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Creep.h"
#include "CreepCamp.generated.h"

UENUM(BlueprintType)
enum class ECampType : uint8
{
	CT_Neutral    UMETA(DisplayName = "Neutral"),
	CT_Cyber      UMETA(DisplayName = "Cyber"),
	CT_Diesel	  UMETA(DisplayName = "Diesel")
};

USTRUCT(BlueprintType)
struct FCaptureVariables
{
	GENERATED_BODY()
	
	FCaptureVariables()
	{
		//Default unless set in blueprint 
		targetCaptureTime = 5.0f;
		captureTime = targetCaptureTime;
		cyberCaptureProgress = 0;
		dieselCaptureProgress = 0;
		bDieselIsCapturing = false;
		bCyberIsCapturing = false;
	}
	/*The more creeps that are in the camp the higher the capture time should be
	When a camp is captured all the creeps associated with it should die */
	UPROPERTY(EditAnywhere, Category = CampVariables)
	float targetCaptureTime;
	UPROPERTY()
	float captureTime;
	UPROPERTY()
	float cyberCaptureProgress;
	UPROPERTY()
	float dieselCaptureProgress;
	UPROPERTY()
	bool bCyberIsCapturing;
	UPROPERTY()
	bool bDieselIsCapturing;
};

USTRUCT(BlueprintType)
struct FSpawningVariables
{
	GENERATED_BODY()

	FSpawningVariables()
	{
		creepSpawnTimerTarget = 5.0f;
		creepSpawnTimer = creepSpawnTimerTarget;
		neutralCreepLimit = 3;
		cyberCreepLimit = 3;
		dieselCreepLimit = 3;
		startCreepAmount = 3;
		creepCount = 0;
	}

	/*Spawn rate for creeps is based on how many are currently at the camp... 
	Less = faster spawn rate, More = slower spawn rate*/
	UPROPERTY()
	int creepCount; 
	UPROPERTY(EditAnywhere, Category = CampVariables)
	int neutralCreepLimit;
	UPROPERTY(EditAnywhere, Category = CampVariables)
	int cyberCreepLimit;
	UPROPERTY(EditAnywhere, Category = CampVariables)
	int dieselCreepLimit;
	UPROPERTY(EditAnywhere, Category = CampVariables)
	int startCreepAmount;
	
	UPROPERTY(EditAnywhere, Category = CampVariables)
	float creepSpawnTimerTarget;
	UPROPERTY()
	float creepSpawnTimer;


};

class AHeroBase;

UCLASS()
class FUSIONPUNKS_API ACreepCamp : public AActor
{
	GENERATED_BODY()
	
public:	
	ACreepCamp();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	void LinkToHeroes(class AHeroBase* hero);

	FName team;
	FORCEINLINE FName GetTeam() const { return team; }

//enum, captureStruct, variablesStruct 
private:
	UPROPERTY(EditAnywhere, Category = Stats)
		ECampType campType;

	UPROPERTY(EditAnywhere, Category = Stats)
		FCaptureVariables captureVariables;
	UPROPERTY(EditAnywhere, Category = Stats)
		FSpawningVariables spawningVariables;

	class AHeroBase* dieselHero;
	class AHeroBase* cyberHero;

//creep class references
private:
	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		 TSubclassOf<class ANeutralCreep> neutralCreepRef;

	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		TSubclassOf<class ACyberCreep> cyberCreepRef;

	UPROPERTY(EditDefaultsOnly, Category = "Creeps")
		TSubclassOf<class ADieselCreep> dieselCreepRef;


//Meshes and triggers 
protected:
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		class UStaticMeshComponent* campMesh; 

	//index for the flag material = 1
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		class UStaticMeshComponent* lampPostMesh;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		UMaterial* lampPostMaterial;

	//0 is neutral, 1 is cyber, 2 is diesel
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		TArray<UMaterialInstance*> FlagMaterials; 
	const int NUMFLAGS = 3; 

	UPROPERTY(EditDefaultsOnly, Category = CollisionComponents)
		class USphereComponent* sphereTrigger;

	////mesh for the ring around the camp
	//UPROPERTY(EditDefaultsOnly, Category = Appearance)
	//	class UStaticMeshComponent* ringMesh;


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

public:
	//Brendon - Note: Move these to structs????
//Camp Capture Functions
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		FORCEINLINE float GetCyberCapturePercentage() const { return captureVariables.cyberCaptureProgress / captureVariables.captureTime; }

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		FORCEINLINE float GetDieselCapturePercentage() const {return captureVariables.dieselCaptureProgress / captureVariables.captureTime;}

		//AI HERO CAMP SEARCH STUFF
		FORCEINLINE int32 GetNumOfCreepsAtCamp() const { return spawningVariables.creepCount; }
		FORCEINLINE  bool GetCampSafety() const { return bSafeToCapture; }
		FORCEINLINE  void SetCampSafety(bool status) { bSafeToCapture = status; }
		FORCEINLINE  void SetCampRecruitedStatus(bool status) { bRecruitedByFromAI = status; }
		FORCEINLINE  bool HasBeenRecruitedFrom()const { return bRecruitedByFromAI; }
		FORCEINLINE  bool IsDieselCapturing()const { return captureVariables.bDieselIsCapturing; }
		FORCEINLINE bool IsCyberCapturing() const { return captureVariables.bCyberIsCapturing; }
		FORCEINLINE  TArray<ACreep*> GetCreepArray() const { return creepArray; }
//Creep Spawning Functions
public:
	void RemoveCreep(ACreep* CreepInCamp);
	void DestroyAllCreeps();

	FORCEINLINE const float GetDistanceValue() const { return distanceValue; }
	FORCEINLINE void SetDistanceValue(float value) { distanceValue = value; }
	FORCEINLINE const ECampType GetCampType() const {return campType;}

//Creep Spawn Locations
protected:
	TArray<ACreep*> creepArray;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep1SpawnLocation;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep2SpawnLocation;

	UPROPERTY(EditAnywhere, Category = SpawnLocation)
		FVector creep3SpawnLocation;

	TArray<FVector> creepSpawnArray;
	float distanceValue;
	
	//FOR HERO AI CAMP SEARCH 
	bool bSafeToCapture = true;
	bool bRecruitedByFromAI = false;
	
protected:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToDieselCamp();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToCyberCamp();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		void SetToNeutralCamp();

	

public:

	ACreep* SendCreepToPlayer(class AHeroBase* Player);

	ACreep* GetNextCreep(class AHeroBase* Hero);

};
