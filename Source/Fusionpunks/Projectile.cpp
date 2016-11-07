// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "Creep.h"
#include "DieselTower.h"
#include "Projectile.h"



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
	
	
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	owningTower = Cast<ADieselTower>(GetOwner());
	
}

void AProjectile::SetDamage(float amount) 
{
	damage = amount;
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
		

	if (enemyType == ETypeOfEnemy::TE_Hero)
	{
		if (enemyHero != nullptr)
		{
			if (!enemyHero->IsRespawning())
			{
				FVector direction = (enemyHero->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				FVector newPos = GetActorLocation() + (direction * DeltaTime * 1000);
				SetActorLocation(newPos);
			}

			else 
			{
				this->Destroy();
			}


		}

		else 
		{
			this->Destroy();
		}
	}



	else if (enemyType == ETypeOfEnemy::TE_Creep)
	{
		if (enemyCreep != nullptr)
		{
			if (enemyCreep->GetHealthAsDecimal() >= 0)
			{
				FVector direction = (enemyCreep->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				FVector newPos = GetActorLocation() + (direction * DeltaTime * 1000);
				SetActorLocation(newPos);
			}

			else
			{
				this->Destroy();
			}


		}

		else
		{
			this->Destroy();
		}
	}


	

}

void AProjectile::SetTarget(class AActor* OtherActor)
{

	enemyCreep = nullptr;
	enemyHero = nullptr;

	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		enemyHero = Cast<AHeroBase>(OtherActor);
		enemyType = ETypeOfEnemy::TE_Hero;
	}

	else if (OtherActor->IsA(ACreep::StaticClass()))
	{
		enemyCreep = Cast<ACreep>(OtherActor);
		enemyType = ETypeOfEnemy::TE_Creep;
	}
	
	
	
	//enemyPlayer = OtherActor;



}

void AProjectile::TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		FDamageEvent DamageEvent;

		float damageTaken = OtherActor->TakeDamage(damage, DamageEvent, NULL, owningTower);
		
		Destroy();
		
	}
}

