// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "HeroBase.h"
#include "BulletBase.h"


// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BulletMeshComponent");
	bulletMeshComponent->bGenerateOverlapEvents;
	bulletMeshComponent->SetSimulatePhysics(false);
	RootComponent = bulletMeshComponent;

	projectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	particleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	particleSystemComp->AttachToComponent(bulletMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);

	InitialLifeSpan = 5.0f;

	Tags.Add(TEXT("Bullet"));
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();

	bulletMeshComponent->OnComponentHit.AddDynamic(this, &ABulletBase::OnBulletHit);
	
	if(IsValid(GetOwner()))
	{
		projectileMovementComp->SetVelocityInLocalSpace(GetOwner()->GetActorForwardVector() * speed);
	}
}

// Called every frame
void ABulletBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );	
}

void ABulletBase::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACreep* creep = Cast<ACreep>(GetOwner());
	AHeroBase* hero = Cast<AHeroBase>(GetOwner());

	if (creep)
	{
		if (!OtherActor->Tags.Contains(creep->GetTeam()))
		{
			OtherActor->TakeDamage(damage, FDamageEvent::FDamageEvent(), creep->GetController(), creep);
			return;
		}
	}
	if (hero)
	{
		if (!OtherActor->Tags.Contains(hero->GetTeam()))
		{
			OtherActor->TakeDamage(damage, FDamageEvent::FDamageEvent(), hero->GetController(), hero);
			return; 
		}
	}
	
}

