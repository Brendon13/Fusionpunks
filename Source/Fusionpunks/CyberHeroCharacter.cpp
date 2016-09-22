// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Fusionpunks.h"
#include "TestCreep.h"
#include "ChainLightning.h"
#include "FusionpunksGameState.h"
#include "CyberHeroCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AMobaCharacter

ACyberHeroCharacter::ACyberHeroCharacter()
{
	maxHealth = 250;
	currentHealth = maxHealth;

	team = FName::FName(TEXT("Cyber"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACyberHeroCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	Super::SetupPlayerInputComponent(InputComponent);
	check(InputComponent);
	
	InputComponent->BindAction("Basic Attack", IE_Pressed, this, &ACyberHeroCharacter::DetermineClickEvent);
	InputComponent->BindAction("Skill1", IE_Pressed, this, &ACyberHeroCharacter::OnSkillPressed);
}


void ACyberHeroCharacter::DetermineClickEvent()
{
	if (skillSelected)
	{
		AActor* currentTarget = UpdateTarget();
		if (currentTarget != NULL) 
		{
			UseSkill(currentTarget);
			UnHighlightTarget(currentTarget);
			skillSelected = false;
		}
	}
	else
	{
		StartAttack();
	}
}


void ACyberHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACyberHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (skillSelected) 
	{
		UpdateTarget();
	}
}


AActor* ACyberHeroCharacter::UpdateTarget()
{
	//UE_LOG(LogTemp, Display, TEXT("Skill Used"));
	if (oldTargetResults.Num() > 0)
	{
		UnHighlightAll(oldTargetResults);
	}

	AActor *closestEnemy;
	FCollisionObjectQueryParams obejctQP;
	obejctQP.AddObjectTypesToQuery(Creeps);
	//obejctQP.AddObjectTypesToQuery()
	//Overlap multi by channel as a sphere (for pick ups?)

	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	QueryParameters.OwnerTag = TEXT("Player");	
	GetWorld()->OverlapMultiByObjectType(skillTargetResults,
		GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(300.f),
		QueryParameters);
	oldTargetResults = skillTargetResults;
	
	if (skillTargetResults.Num() == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("No Enemies Nearby"));
		return NULL;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Enemy Found"));
		closestEnemy = skillTargetResults[0].GetActor();

		for (int i = 0; i < skillTargetResults.Num(); i++)
		{
			if (GetDistanceTo(skillTargetResults[i].GetActor()) <= GetDistanceTo(closestEnemy))
			{
				closestEnemy = skillTargetResults[i].GetActor();
			}
		}

		HighlightTarget(closestEnemy, skillTargetResults);
		return closestEnemy;
	}
}

void ACyberHeroCharacter::OnSkillPressed()
{
	if (!skillSelected)
	{
		skillSelected = true;
	}
	else 
	{
		skillSelected = false;
		AActor* currentTarget = UpdateTarget();

		if (currentTarget != NULL)
		{
			UnHighlightTarget(currentTarget);
		}
	}
}

void ACyberHeroCharacter::UseSkill(AActor* enemy)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Instigator = this;

	FVector spawnLoc;
	spawnLoc = GetActorLocation();
	spawnLoc.Z = spawnLoc.Z + 200;
   	AChainLightning* lightning =  GetWorld()->SpawnActor
		<AChainLightning>(chainLightningAbility,
		spawnLoc,
		FRotator::ZeroRotator);
	
	lightning->AddAffectedActor(enemy);
	lightning->SetBeamPoints(Cast<AActor>(this),enemy);	
	lightning->Use();

}

void ACyberHeroCharacter::AddAffectedActor(AActor* enemy)
{
	affectedActors.Add(enemy);
}

bool ACyberHeroCharacter::IsAffected(AActor* enemy)
{
	return affectedActors.Contains(enemy);
}

void ACyberHeroCharacter::UnHighlightTarget(AActor* enemy)
{
	if (enemy->IsA(ATestCreep::StaticClass()))
	{
		Cast<ATestCreep>(enemy)->GetMesh()->SetRenderCustomDepth(false);
	}
}

void ACyberHeroCharacter::HighlightTarget(AActor* enemy, TArray<FOverlapResult> enemies)
{
	if (enemy->IsA(ATestCreep::StaticClass()))
	{
		Cast<ATestCreep>(enemy)->GetMesh()->SetRenderCustomDepth(true);
		Cast<ATestCreep>(enemy)->GetMesh()->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;		
		
	}

	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemy != enemies[i].GetActor() && enemies[i].GetActor()->IsA(ATestCreep::StaticClass()))
		{
			Cast<ATestCreep>(enemies[i].GetActor())->GetMesh()->SetRenderCustomDepth(false);
		}

	}
}

void ACyberHeroCharacter::UnHighlightAll(TArray<FOverlapResult> enemies)
{
	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemies[i].GetActor()->IsA(ATestCreep::StaticClass()))
		{
			Cast<ATestCreep>(enemies[i].GetActor())->GetMesh()->SetRenderCustomDepth(false);
		}
	}
}

void ACyberHeroCharacter::LevelUp()
{
	if (currentLevel < maxLevel)
	{
		Super::LevelUp();

		//notify GameState we leveled up
		if (GetWorld())
		{
			AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
			gameState->CyberLevelUp();
		}
	}
}

