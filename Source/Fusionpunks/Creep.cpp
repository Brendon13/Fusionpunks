// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Creep.h"

ACreep::ACreep()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	creepSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	creepSkeletalMeshComp->SetSimulatePhysics(true);
	creepSkeletalMeshComp->bGenerateOverlapEvents = true; 
	RootComponent = creepSkeletalMeshComp;

	const ConstructorHelpers::FObjectFinder<UBlueprint>
		CreepHealthBarFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/CreepHealthBarWidget_BP.CreepHealthBarWidget_BP'"));

	if (CreepHealthBarFinder.Object != nullptr)
	{
		CreepHealthBarWidgetClass = Cast<UClass>(CreepHealthBarFinder.Object->GeneratedClass);
		widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
		widgetComponent->SetWidgetClass(CreepHealthBarWidgetClass);
		widgetComponent->SetSimulatePhysics(false);
		widgetComponent->bGenerateOverlapEvents = false;
		widgetComponent->AttachToComponent(creepSkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
	}

	//Will have to change current level based on players level when spawned from a controlled camp 
	//i.e. Diesel hero is level 9 -> creep should also be level 9 
	currentLevel = 1;
	maxLevel = 10;
	maxHealth = 10;

	bBelongsToCamp = false;
	Tags.Add(TEXT("Creep"));
}

void ACreep::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;

	if (GetWorld())
	{
		localPlayer = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	}
	
	if (Cast<UCreepHealthbarWidget>(widgetComponent->GetUserWidgetObject()))
	{
		UCreepHealthbarWidget* thisCreepHealthBarWidget = Cast<UCreepHealthbarWidget>(widgetComponent->GetUserWidgetObject());
		thisCreepHealthBarWidget->SetOwningCreep(this);
	}
}

void ACreep::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	widgetCompRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), localPlayer->GetActorLocation());
	widgetComponent->SetWorldRotation(widgetCompRotation);
}

void ACreep::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

float ACreep::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	currentHealth -= Damage;
	if (currentHealth <= 0)
	{
		if (bBelongsToCamp)
		{
			if (creepCampHome != nullptr)
			{
				creepCampHome->MinusOneFromCreepCamp();
				this->Destroy();
			}
		}
	}
	return Damage;
}

void ACreep::LevelUp()
{
	if (currentLevel + 1 <= maxLevel)
	{
		currentLevel++;
		maxHealth += healthIncreasePerLevel;
		damage += damageIncreasePerLevel;
	}
}

void ACreep::SetCreepCampHome(ACreepCamp* home, bool BelongsToCamp)
{
	creepCampHome = home;
	bBelongsToCamp = BelongsToCamp;
}

