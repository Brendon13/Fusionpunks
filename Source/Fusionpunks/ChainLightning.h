// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilityBase.h"
#include "TestCreep.h"
//#include "MobaCharacter.h"
#include "ChainLightning.generated.h"

UCLASS()
class FUSIONPUNKS_API AChainLightning : public AAbilityBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChainLightning();

	bool hasBeenTriggered = false;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetBeamPoints(AActor* a, AActor* b);
	
	virtual void Use() override;

	void AddAffectedActor(AActor* enemy);
	
	float destroyTimer = 0;

	bool startDestroyTimer = false;

	

private:
	AActor* source, *target;
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* beam;

	UPROPERTY(EditAnywhere)
		UShapeComponent* boxCollider;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;
	
	UPROPERTY(EditAnywhere, category = Ability)
		TSubclassOf<AAbilityBase> chainLightningAbility;
	
	TArray<AActor*> affectedActors;

private:
	UFUNCTION()
		void TriggerEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void CheckForNearbyEnemies();
	
	

};
