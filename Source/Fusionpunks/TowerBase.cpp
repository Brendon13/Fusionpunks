// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FloatingDamageWidget.h"
#include "HeroBase.h"
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

	/*const ConstructorHelpers::FObjectFinder<UBlueprint>
		FloatingDamageWidgetFinder(TEXT("WidgetBlueprint'/Game/UI/FloatingDamageWidget_BP.FloatingDamageWidget_BP'"));

	if (FloatingDamageWidgetFinder.Object != nullptr)
	{
		FloatingDamageWidgetClass = Cast<UClass>(FloatingDamageWidgetFinder.Object->GeneratedClass);
	}*/

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
	if (enemyUnits.Contains(OtherActor))
		enemyUnits.Remove(OtherActor);
	
}

void ATowerBase::TriggerEnter(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		if (ActorHasTag("Diesel") && !OtherActor->ActorHasTag("Diesel"))
			enemyUnits.Add(OtherActor);
		else if (ActorHasTag("Cyber") && !OtherActor->ActorHasTag("Cyber"))
			enemyUnits.Add(OtherActor);
	}
}

float ATowerBase::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currHP -= DamageAmount;

	if (!DamageCauser->ActorHasTag("AI") && !DamageCauser->ActorHasTag("Creep") && FloatingDamageWidgetClass)
	{
		UFloatingDamageWidget* floatingDamageWidget = CreateWidget<UFloatingDamageWidget>(GetWorld()->GetFirstPlayerController(), FloatingDamageWidgetClass);
		floatingDamageWidget->SetAlignmentInViewport(FVector2D::FVector2D(0.5f, 0.5f));
		floatingDamageWidget->SetIncDamage(DamageAmount);
		floatingDamageWidget->SetOwningActor(this);
		floatingDamageWidget->AddToViewport();
	}

	UE_LOG(LogTemp, Log, TEXT("Tower took %f damage."), DamageAmount);
	if (currHP <= 0) 
	{
		AHeroBase* hero = Cast<AHeroBase>(DamageCauser);
		if (hero)
		{
			UE_LOG(LogTemp, Log, TEXT("%i experence rewarded"), XPKillReward);
			hero->AddToExperience(XPKillReward);
		}

		CleanUp();
		Destroy();
	}
	return DamageAmount;
}

float ATowerBase::GetHpPercent()
{
	return currHP / maxHP;
}

