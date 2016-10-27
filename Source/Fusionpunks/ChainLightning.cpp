// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "ChainLightning.h"


// Sets default values
AChainLightning::AChainLightning()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ps(TEXT("ParticleSystem'/Game/ParticleEffects/LaserBeam2.LaserBeam2'"));
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Colider"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = boxCollider;
	mesh->AttachToComponent(boxCollider, FAttachmentTransformRules::KeepRelativeTransform);
	beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Particle"));
	beam->SetTemplate(ps.Object);
	boxCollider->bGenerateOverlapEvents = true;
	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &AChainLightning::TriggerEnter);
}

// Called when the game starts or when spawned
void AChainLightning::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AChainLightning::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (target) 
	{
		FVector newLocation = FMath::Lerp(GetActorLocation(), target->GetActorLocation(), 15.0f * DeltaTime);
		SetActorLocation(newLocation);
		beam->SetBeamSourcePoint(0,source->GetActorLocation(),0);
		beam->SetBeamTargetPoint(0, target->GetActorLocation(), 0);
	}


	if (startDestroyTimer) 
	{
		destroyTimer += DeltaTime;

		if (destroyTimer >= 1.5f)
		{
			this->Destroy();
		}

	}
	
	
}

bool AChainLightning::Ability() 
{
	beam->SetBeamSourcePoint(0, source->GetActorLocation(), 0);
	beam->SetBeamTargetPoint(0, target->GetActorLocation(), 0);
	
	return true; 
}

void AChainLightning::SetBeamPoints(AActor* a, AActor* b)
{
	source = a;
	target = b;
	
}
void AChainLightning::TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
									int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		if (!hasBeenTriggered) {
			UE_LOG(LogTemp, Display, TEXT("Chain Lightning Trigger"));
			CheckForNearbyEnemies();
			hasBeenTriggered = true;
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(20, DamageEvent, NULL, this);
		}
	}	
}
void AChainLightning::CheckForNearbyEnemies()
{

	AActor *closestEnemy;
	FCollisionObjectQueryParams obejctQP;
	obejctQP.AddObjectTypesToQuery(Creeps);
	obejctQP.AddObjectTypesToQuery(Hero);
	//Overlap multi by channel as a sphere (for pick ups?)
	FCollisionQueryParams QueryParameters;
	if (GetOwner() != nullptr)
	{
		if (GetOwner()->IsA(AHeroBase::StaticClass()))
			QueryParameters.AddIgnoredActor(GetOwner());
	}
	QueryParameters.AddIgnoredActor(target);
	QueryParameters.OwnerTag = TEXT("Player");
	FCollisionResponseParams ResponseParameters;

	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,
		target->GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(750),
		QueryParameters);

	if (Results.Num() == 0) {
		UE_LOG(LogTemp, Display, TEXT("No Enemies Nearby"));
		
	}
	else
	{
		closestEnemy = Results[0].GetActor();
		for (int i = 0; i < Results.Num(); i++)
	    {
			if (!affectedActors.Contains(Results[i].GetActor()))
			{
				closestEnemy = Results[i].GetActor();
			}

		}
		
		if (!affectedActors.Contains(closestEnemy))
		{
			UE_LOG(LogTemp, Display, TEXT("Found Unaffected NearbyEnemy"));
			FVector spawnLoc;
			spawnLoc = target->GetActorLocation();
			spawnLoc.Z = spawnLoc.Z + 200;
			AChainLightning* lightning = GetWorld()->SpawnActor
				<AChainLightning>(chainLightningAbility,
					spawnLoc,
					FRotator::ZeroRotator);
			for (int i = 0; i < affectedActors.Num(); i++)
			{
				lightning->AddAffectedActor(affectedActors[i]);
			}
			lightning->AddAffectedActor(closestEnemy);
			lightning->SetBeamPoints(target, closestEnemy);
			lightning->Use();
		}
		

	
	}
	startDestroyTimer = true;
	
}

void AChainLightning::AddAffectedActor(AActor* enemy)
{
	if (GetOwner() != nullptr)
	{
		if (GetOwner()->IsA(AHeroBase::StaticClass()))
			affectedActors.Add(GetOwner());
	}
	affectedActors.Add(enemy);
}
