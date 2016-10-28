// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HealOverTime.h"
#include "HeroBase.h"
#include "HealingWell.h"


// Sets default values
AHealingWell::AHealingWell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = meshComp;
	decalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	decalComp->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	healRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Healing Radius"));
	healRadius->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	healRadius->bGenerateOverlapEvents = true;
	healRadius->OnComponentBeginOverlap.AddDynamic(this, &AHealingWell::TriggerEnter);
	healRadius->OnComponentEndOverlap.AddDynamic(this, &AHealingWell::TriggerExit);
	


}

// Called when the game starts or when spawned
void AHealingWell::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	healingEffect = GetWorld()->SpawnActor<AHealOverTime>(healingEffectClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
}

// Called every frame
void AHealingWell::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AHealingWell::TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		AHeroBase* hero = Cast<AHeroBase>(OtherActor);
	
		if (ActorHasTag("Diesel") && OtherActor->ActorHasTag("Diesel"))
		{
			hero->SetInsideHealingWell(true);
			healingEffect->SetTotalHealthValue(50,0.1);
			healingEffect->StartTimer(0.1, OtherActor);
		}

		else if (ActorHasTag("Cyber") && OtherActor->ActorHasTag("Cyber"))
		{
			hero->SetInsideHealingWell(true);
			UE_LOG(LogTemp, Error, TEXT("CYBER HEALING WELL ACTIVATED"))
			healingEffect->SetTotalHealthValue(50,0.1);
			healingEffect->StartTimer(0.1, OtherActor);
		}
	
	}
}
void AHealingWell::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  
	if (OtherActor->IsA(AHeroBase::StaticClass()))
	{
		AHeroBase* hero = Cast<AHeroBase>(OtherActor);
		if (ActorHasTag("Diesel") && OtherActor->ActorHasTag("Diesel"))
		{
			hero->SetInsideHealingWell(false);
			healingEffect->StopTimer();
			if (hero->GetCurrentHealth() >= hero->GetMaxHealth())
			{
				hero->ResetHealth();
			}
		
		}

		else if (ActorHasTag("Cyber") && OtherActor->ActorHasTag("Cyber"))
		{
			hero->SetInsideHealingWell(false);
			healingEffect->StopTimer();
			if (hero->GetCurrentHealth() >= hero->GetMaxHealth())
			{
				hero->ResetHealth();
			}
		}
	}
}
