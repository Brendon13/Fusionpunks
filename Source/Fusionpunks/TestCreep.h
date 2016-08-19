// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TestCreep.generated.h"

UCLASS()
class FUSIONPUNKS_API ATestCreep : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestCreep();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UStaticMeshComponent* GetMesh();
	

private:
	UPROPERTY(EditAnywhere)
		UShapeComponent* boxCollider;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh;



	
	
};
