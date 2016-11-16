// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETypeOfEnemy : uint8
{
	TE_Creep UMETA(DisplayName = "Creep"),
	TE_Hero UMETA(DisplayName = "Hero"),
	TE_NoTarget UMETA(DisplayName = "NoTaget")
	
};


UCLASS()
class FUSIONPUNKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION()
		void SetTarget(class AActor* OtherActor);

	void SetDamage(float amount);
protected:
	FVector target;
	
	ETypeOfEnemy enemyType = ETypeOfEnemy::TE_NoTarget;

	class ACreep* enemyCreep;
	class AHeroBase*  enemyHero;
	class ADieselTower* owningTower;
	float damage;
	float deathTimer = 0;


	UPROPERTY(EditAnywhere)
		UShapeComponent* sphereShape;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* sphereDisplay;
	UFUNCTION()
		void TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
