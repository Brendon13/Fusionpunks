// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HealthBarWidgetComponent.h"
#include "GameFramework/Actor.h"
#include <Classes/Kismet/KismetMathLibrary.h>
#include "BaseHealthBar.h"
#include "Base.generated.h"

UCLASS()
class FUSIONPUNKS_API ABase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	

	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	float GetHpPercent();

	
private:
	UPROPERTY(EditDefaultsOnly)
		UShapeComponent* rootCldr;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* baseMesh;

	UPROPERTY(EditDefaultsOnly)
		UHealthBarWidgetComponent *healthBar;
	
	UPROPERTY(EditDefaultsOnly)
		float maxHP = 100;

		float currHP;
   	
	APlayerController* playerCam;

	AActor* playerChar;
	
	
};
