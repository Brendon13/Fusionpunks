// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "FloatingDamageWidget.h"
#include "Creep.h"


ACreep::ACreep()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	creepSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	creepSkeletalMeshComp->SetSimulatePhysics(false);
	creepSkeletalMeshComp->bGenerateOverlapEvents = true; 
	RootComponent = creepSkeletalMeshComp;

	movementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

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

	const ConstructorHelpers::FObjectFinder<UBlueprint>
		FloatingDamageWidgetFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/FloatingDamageWidget_BP.FloatingDamageWidget_BP'"));
	if (FloatingDamageWidgetFinder.Object != nullptr)
	{
		FloatingDamageWidgetClass = Cast<UClass>(FloatingDamageWidgetFinder.Object->GeneratedClass);
	}

	const ConstructorHelpers::FClassFinder<ACreepAIController> AIContFinder(TEXT("Class'/Script/Fusionpunks.CreepAIController'"));
	if (IsValid(AIContFinder.Class))
	{
		AIControllerClass = AIContFinder.Class;
	}
	//bUseControllerRotationYaw = false; 

	//NOTE::BRENDON - Will have to change current level based on players level when spawned from a controlled camp 
	//i.e. Diesel hero is level 9 -> creep should also be level 9 
	currentLevel = 1;
	maxLevel = 10;
	maxHealth = 10;

	patrolRadius = 1000.0f;

	bBelongsToCamp = false;
	Tags.Add(TEXT("Creep"));
}

void ACreep::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;

	if (GetWorld())
	{
		AiController = Cast<ACreepAIController>(GetWorld()->SpawnActor<ACreepAIController>(AIControllerClass));
		AiController->Possess(this);
		AiController->GetBlackboardComponent()->SetValueAsObject(TEXT("SelfPawn"), this);
		AiController->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		AiController->GetBlackboardComponent()->SetValueAsBool(TEXT("belongsToCamp"), bBelongsToCamp);

		if (creepCampHome != nullptr)
		{
			AiController->GetBlackboardComponent()->SetValueAsObject(TEXT("CreepCampHome"), creepCampHome);
		}
		localPlayer = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	}

	//AiController = Cast<ACreepAIController>(GetController());
	/*NOTE::BRENDON -> Creep camp should set this to true when it creates them. Set to false incase we want to spawn
	them from the player or in another situation*/
	//blackboardComponent->SetValueAsBool(TEXT("belongsToCamp"), false);
	
	if (Cast<UCreepHealthbarWidget>(widgetComponent->GetUserWidgetObject()))
	{
		UCreepHealthbarWidget* thisCreepHealthBarWidget = Cast<UCreepHealthbarWidget>(widgetComponent->GetUserWidgetObject());
		thisCreepHealthBarWidget->SetOwningCreep(this);
	}
}

void ACreep::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (localPlayer)
	{
		UCameraComponent* cameraComponent = Cast<UCameraComponent>(localPlayer->GetComponentByClass(UCameraComponent::StaticClass()));
		widgetCompRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), cameraComponent->GetComponentLocation());
		widgetCompRotation.Pitch = 0;
		widgetComponent->SetWorldRotation(widgetCompRotation);
	}

	
}

void ACreep::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

float ACreep::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	currentHealth -= Damage;

	UFloatingDamageWidget* floatingDamageWidget = CreateWidget<UFloatingDamageWidget>(GetWorld()->GetFirstPlayerController(), FloatingDamageWidgetClass);
	floatingDamageWidget->SetIncDamage(Damage);
	floatingDamageWidget->AddToViewport();

	if (currentHealth <= 0)
	{
		if (bBelongsToCamp && creepCampHome != nullptr)
		{
			creepCampHome->RemoveCreep(this);
			//creepCampHome->MinusOneFromCreepCamp();
		}
		this->Destroy();
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

void ACreep::SetCreepCampHome(ACreepCamp* home, bool BelongsToCamp = false)
{
	creepCampHome = home;
	bBelongsToCamp = BelongsToCamp;
	//AiController->GetBlackboardComponent()->SetValueAsBool(TEXT("belongsToCamp"), BelongsToCamp);
}

UFUNCTION()
float ACreep::GetPatrolRadius()
{
	return patrolRadius;
}

UFUNCTION()
ACreepCamp* ACreep::GetCreepCampHome() const
{
	if (creepCampHome)
	{
		return creepCampHome;
	}
	else
	{
		return nullptr;
	}
}

