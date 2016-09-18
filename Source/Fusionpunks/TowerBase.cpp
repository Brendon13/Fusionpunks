// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "TowerBase.h"


// Sets default values
ATowerBase::ATowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootCldr = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = rootCldr;
	visualTower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualCube"));
	visualTower->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	radius = CreateDefaultSubobject<USphereComponent>(TEXT("Radius"));
	radius->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	radius->bGenerateOverlapEvents = true;
	radius->OnComponentBeginOverlap.AddDynamic(this, &ATowerBase::TriggerEnter);
	radius->OnComponentEndOverlap.AddDynamic(this, &ATowerBase::TriggerExit);

	//static ConstructorHelpers::FObjectFinder<UParticleSystem> ps(TEXT("ParticleSystem'/Game/BluePrints/LaserBeam.LaserBeam'"));
	//beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Particle"));
	//beam->SetTemplate(ps.Object);
	//beam->AttachTo(RootComponent);
	spawnTimer = 0;

	bCanBeDamaged = true;
	currHP = maxHP;

	//healthBar = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBar"));
	//healthBar->AttachTo(RootComponent);
}

void ATowerBase::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	enemyUnits.Remove(OtherActor);
}

void ATowerBase::TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		enemyUnits.Add(OtherActor);
	}
}

float ATowerBase::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currHP -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("Tower took %f damage."), DamageAmount);
	if (currHP <= 0) {
		Destroy();
	}
	return DamageAmount;
}

float ATowerBase::GetHpPercent()
{
	return currHP / maxHP;
}

