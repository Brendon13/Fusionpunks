// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "TowerDamage.h"
#include "CyberTower.h"


ACyberTower::ACyberTower() 
{
	beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Particle"));	
	beam->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Tags.Add(TEXT("Cyber"));
}

void ACyberTower::BeginPlay()
{
	Super::BeginPlay();
	beam->bAutoActivate = false;
	beam->SecondsBeforeInactive = 0;
	sourceLocation = GetActorLocation();
	sourceLocation.Z = 200;
	towerDMG = GetWorld()->SpawnActor<ATowerDamage>(towerDMGPrefab, FVector::ZeroVector, FRotator::ZeroRotator);

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
				towerDMG->ApplyEffect(0.1f, enemyUnits[0]);
				bIsDealingDMG = true;
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
