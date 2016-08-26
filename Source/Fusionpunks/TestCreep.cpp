// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "TestCreep.h"


// Sets default values
ATestCreep::ATestCreep()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Colider"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = boxCollider;
	mesh->AttachToComponent(boxCollider, FAttachmentTransformRules::KeepRelativeTransform); (boxCollider);

}

// Called when the game starts or when spawned
void ATestCreep::BeginPlay()
{
	Super::BeginPlay();
	
}

UStaticMeshComponent* ATestCreep::GetMesh() 
{
	return mesh;
}



