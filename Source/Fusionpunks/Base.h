// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HealthBarWidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include <Classes/Kismet/KismetMathLibrary.h>
#include "Base.generated.h"

UCLASS()
class FUSIONPUNKS_API ABase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABase();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	float GetHpPercent();
	
protected:
	/*UPROPERTY(EditDefaultsOnly)
		UShapeComponent* rootCldr;*/

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* baseMesh;

	UPROPERTY(EditDefaultsOnly)
		UHealthBarWidgetComponent *healthBar;
	
	UPROPERTY(EditDefaultsOnly)
		float maxHP = 100;

		float currHP;
   	
	APlayerController* playerCam;

	AActor* playerChar;

public:
	virtual float TakeDamage(float DamageAmount,
							struct FDamageEvent const & DamageEvent,
							class AController * EventInstigator,
							AActor * DamageCauser) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass;
	
	UPROPERTY(EditAnywhere, Category = Cutscenes)
		class ULevelSequencePlayer* sequencePlayer;
	
	UPROPERTY(EditAnywhere, Category = Cutscenes)
		class ALevelSequenceActor* sequenceActor;
};
