// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"

#include "Base.h"



// Sets default values
ABase::ABase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootCldr = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = rootCldr;
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	healthBar = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBar"));
	healthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	bCanBeDamaged = true;
	currHP = maxHP;
	
	
	
	
}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();
	
	playerCam = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Cast<UBaseHealthBar>(healthBar->GetUserWidgetObject()))
	{
		UBaseHealthBar* hb = Cast<UBaseHealthBar>(healthBar->GetUserWidgetObject());
		hb->SetOwningActor(this);
		UE_LOG(LogTemp, Log, TEXT("NOT NULL"));
	}
	
   
}

// Called every frame
void ABase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), playerChar->GetActorLocation());
	healthBar->SetWorldRotation(newRotation);
	
}


float ABase::TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator,AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currHP -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("Base took %f damage."), DamageAmount);
    if (currHP <= 0) {
		Destroy();
	}
	return DamageAmount;

}

float ABase::GetHpPercent() {
	
	return currHP / maxHP;
}

