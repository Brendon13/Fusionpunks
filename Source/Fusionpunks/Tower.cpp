// Fill out your copyright notice in the Description page of Project Settings.


#include "Fusionpunks.h"
#include "Tower.h"

// OLD TOWER CLASS... NOT USING ANYMORE!!!!!!
// OLD TOWER CLASS... NOT USING ANYMORE!!!!!!
// OLD TOWER CLASS... NOT USING ANYMORE!!!!!!

// Sets default values
ATower::ATower()
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
	radius->OnComponentBeginOverlap.AddDynamic(this, &ATower::TriggerEnter);	
	radius->OnComponentEndOverlap.AddDynamic(this, &ATower::TriggerExit);

	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ps(TEXT("ParticleSystem'/Game/BluePrints/LaserBeam.LaserBeam'"));
	beam = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Beam Particle"));
	beam->SetTemplate(ps.Object);
	beam->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);


	bCanBeDamaged = true;
	currHP = maxHP;
	
	healthBar = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBar"));
	healthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//static ConstructorHelpers::FObjectFinder<UUserWidget> hbWidget(TEXT("WidgetBlueprint'/Game/BluePrints/UnitHealthBar.UnitHealthBar'"));
	/*if (hbWidget.Object != nullptr) 
	{
		UE_LOG(YourLog, Log, TEXT("Found WIDGET"));
		healthBar->SetWidgetClass (hbWidget.Object->GetClass());
		
	}
	*/
	
	
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	
	Super::BeginPlay();
	playerCam = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	myLocation = GetActorLocation();
	spawnTimer = 0;
	beam->bAutoActivate = false;
	beam->SecondsBeforeInactive = 0;
	if (Cast<UBaseHealthBar>(healthBar->GetUserWidgetObject()))
	{
		UBaseHealthBar* hb = Cast<UBaseHealthBar>(healthBar->GetUserWidgetObject());
		hb->SetOwningActor(this);
		UE_LOG(LogTemp, Log, TEXT("NOT NULL"));
	}
}

// Called every frame
void ATower::Tick( float DeltaTime )
{

	Super::Tick( DeltaTime );
	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), playerChar->GetActorLocation());
	healthBar->SetWorldRotation(newRotation);
	if (bIsCannonTower) {
		if (bHasSpawed && currProjectile->IsActorBeingDestroyed())
		{
			bHasSpawed = false;
		}

		spawnTimer += DeltaTime;
		if (enemyUnits.Num() > 0 && !bHasSpawed && spawnTimer >= 1)
		{
			if (enemyUnits[0]->IsA(ACharacter::StaticClass()))
			{
				UE_LOG(LogTemp, Log, TEXT("Attacking Player!"));
				SpawnProjectile();

				if (currProjectile)
				{
					currProjectile->SetTarget(enemyUnits[0]);
				}
			}
		}
	}
	else
	{
		if (enemyUnits.Num() > 0)
		{
			if (enemyUnits[0]->IsA(ACharacter::StaticClass()))
			{
				//UE_LOG(YourLog, Log, TEXT("Attacking Player!"));
				FVector spawnLocation = GetActorLocation();
				spawnLocation.Z = 200;
				//drawer->DrawLine(spawnLocation, enemyUnits[0]->GetActorLocation(), FLinearColor::Red, 0, 5,0.0185f);
				
				if (!beam->bWasActive) {
					beam->Activate();
					beam->SetVisibility(true);
					beam->bWasDeactivated = false;
					beam->bWasActive = true;
					UE_LOG(LogTemp, Log, TEXT("Activating Laser Beam!"));
				}
				beam->SetBeamSourcePoint(0, spawnLocation, 0);
				beam->SetBeamTargetPoint(0, enemyUnits[0]->GetActorLocation(), 0);
				
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
		    }
		
		

	}

		



}


void ATower::TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
	//	UE_LOG(YourLog, Log, TEXT("Attacking Player!"));
	//	SpawnProjectile();

		//if (currProjectile) 
		//{
		//	currProjectile->SetTarget(OtherActor);
		//}

		//bIsEnemyUnit = true;
		enemyUnits.Add(OtherActor);
	}
	

}
void ATower::TriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//bIsEnemyUnit = false;
	enemyUnits.Remove(OtherActor);
}
void ATower::SpawnProjectile() 
{
	if (whatToSpawn != NULL) 
	{
		UWorld* const world = GetWorld();
		if (world && !bHasSpawed)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector spawnLocation = GetActorLocation();
			spawnLocation.Z = 350;
			

			currProjectile = world->SpawnActor<AProjectile>(whatToSpawn, spawnLocation,FRotator(0,0,0),SpawnParams);
			bHasSpawed = true;
			spawnTimer = 0;
		}
	}

}
float ATower::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currHP -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("Tower took %f damage."), DamageAmount);
	if (currHP <= 0) {
		Destroy();
	}
	return DamageAmount;
}
float ATower::GetHpPercent() 
{
	return currHP / maxHP;
}
