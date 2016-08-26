// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "DustParticleEffect.h"


// Sets default values
ADustParticleEffect::ADustParticleEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ps(TEXT("ParticleSystem'/Game/ParticleEffects/Stomp_Smoke.Stomp_Smoke'"));
	dustParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Dust Particle Effect"));
	dustParticleEffect->SetTemplate(ps.Object);
	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ADustParticleEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADustParticleEffect::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

