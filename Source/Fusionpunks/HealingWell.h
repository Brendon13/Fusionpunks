// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HealingWell.generated.h"

UCLASS()
class FUSIONPUNKS_API AHealingWell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealingWell();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:	
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* meshComp;
	
	UPROPERTY(EditDefaultsOnly)
		USphereComponent* healRadius;
	
	UPROPERTY(EditDefaultsOnly)
		UDecalComponent* decalComp;
		
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf <class AHealOverTime> healingEffectClass;
	
	class AHealOverTime* healingEffect;
	
	UFUNCTION()
		void TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
		void TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
