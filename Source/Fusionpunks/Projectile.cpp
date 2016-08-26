// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Projectile.h"
#include "MobaCharacter.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphereShape = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = sphereShape;
	sphereDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	sphereDisplay->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	sphereShape->bGenerateOverlapEvents = true;
	sphereShape->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::TriggerEnter);
	
	
	//sphereShape->bGenerateOverlapEvents = true;
	//sphereShape->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::TriggerEnter);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (enemyPlayer) 
	{
		FVector direction = (enemyPlayer->GetActorLocation() - GetActorLocation() ).GetSafeNormal();
		FVector newPos = GetActorLocation() + (direction * DeltaTime*750);
		SetActorLocation(newPos);
	}

	

}

void AProjectile::SetTarget(class AActor* OtherActor)
{
	enemyPlayer = OtherActor;
}

void AProjectile::TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		//OtherActor->TakeDamage(25.0f);
		Destroy();
		
	}
}

