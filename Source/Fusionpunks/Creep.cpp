// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"


// Sets default values
ACreep::ACreep()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACreep::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACreep::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ACreep::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

