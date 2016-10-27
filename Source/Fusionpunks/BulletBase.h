// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

UCLASS()
class FUSIONPUNKS_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	FORCEINLINE float GetBulletSpeed() const { return speed; }

protected:
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* bulletMeshComponent; 

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float damage;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float speed;

	UPROPERTY(EditDefaultsOnly)
		UProjectileMovementComponent* projectileMovementComp;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* particleSystemComp;

	UFUNCTION()
	void OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
