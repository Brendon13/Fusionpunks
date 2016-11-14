// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "LightningTowerDamage.h"
#include "HeroBase.h"
#include "Creep.h"
#include "CyberTower.h"


ACyberTower::ACyberTower() 
{
	beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Particle"));	
	beam->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	damage = 2.0f;
	damageEverySeconds = 0.1f;

	Tags.Add(TEXT("Cyber"));
}

void ACyberTower::BeginPlay()
{
	Super::BeginPlay();
	beam->bAutoActivate = false;
	beam->SecondsBeforeInactive = 0;
	sourceLocation = GetActorLocation();
	sourceLocation.Z += 500;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	towerDMG = GetWorld()->SpawnActor<ALightningTowerDamage>(towerDMGPrefab, FVector::ZeroVector, FRotator::ZeroRotator,SpawnParams);

}

void ACyberTower::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::TriggerExit(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (enemyUnits.Num() == 0)
	{
		if (IsValid(towerDMG))
		{
			towerDMG->StopTimer();
		}
		
		bIsDealingDMG = false;
	}
}

void ACyberTower::CleanUp() 
{
	towerDMG->Destroy();
}
// Called every frame
void ACyberTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (enemyUnits.Num() > 0)
	{
		if (enemyUnits[0]->IsA(ACharacter::StaticClass()))
		{
			//UE_LOG(YourLog, Log, TEXT("Attacking Player!"));

			if (!beam->bWasActive) {
				beam->Activate();
				beam->SetVisibility(true);
				beam->bWasDeactivated = false;
				beam->bWasActive = true;
				UE_LOG(LogTemp, Log, TEXT("Activating Laser Beam!"));
			}
			beam->SetBeamSourcePoint(0, sourceLocation, 0);
			beam->SetBeamTargetPoint(0, enemyUnits[0]->GetActorLocation(), 0);
			
			if (!bIsDealingDMG)
			{
				towerDMG->StartTimer(damageEverySeconds, enemyUnits[0]);
				bIsDealingDMG = true;
			}

			if (bIsDealingDMG && enemyUnits[0]->IsA(AHeroBase::StaticClass()))
			{
				if (enemyHero == nullptr)
				{
					enemyHero = Cast<AHeroBase>(enemyUnits[0]);
				}
				if (enemyHero != nullptr && enemyHero->GetPlayerHealthAsDecimal() <= 0)
				{
					towerDMG->StopTimer();
					bIsDealingDMG = false;
					enemyHero = nullptr;
				}

			}


			else if (bIsDealingDMG && enemyUnits[0]->IsA(ACreep::StaticClass()))
			{
				if (enemyCreep == nullptr)
				{
					enemyCreep = Cast<ACreep>(enemyUnits[0]);
				}
				if (enemyCreep != nullptr && enemyCreep->GetBIsDead())
				{
					towerDMG->StopTimer();
					bIsDealingDMG = false;
					enemyCreep = nullptr;
				}

			}
		}
	}

	else
	{

		beam->SetVisibility(false);
		if (!beam->bWasDeactivated) {
			beam->Deactivate();
			beam->bWasDeactivated = true;
			beam->bWasActive = false;
			UE_LOG(LogTemp, Log, TEXT("DEActivating Laser Beam!"));
		}

		if (bIsDealingDMG)
		{
			towerDMG->StopTimer();
			bIsDealingDMG = false;
		}
	}
}
