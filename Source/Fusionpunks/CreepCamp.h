// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CreepCamp.generated.h"

UCLASS()
class FUSIONPUNKS_API ACreepCamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreepCamp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	FString status;


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
protected:
	UPROPERTY(EditAnywhere, Category = CampVariables)
		float captureTime;
public:
	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetCyberCapturePercentage();

	UFUNCTION(BlueprintCallable, Category = CampFunctions)
		float GetDieselCapturePercentage();
private:
	float cyberCaptureProgress;
	float dieselCaptureProgress;
	bool bCyberIsCapturing;
	bool bDieselIsCapturing;


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
