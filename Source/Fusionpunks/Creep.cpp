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

	/*creepSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	creepSkeletalMeshComp->SetSimulatePhysics(false);
	creepSkeletalMeshComp->bGenerateOverlapEvents = true; 
	RootComponent = creepSkeletalMeshComp;*/

	//movementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	bUseControllerRotationYaw = true;

	const ConstructorHelpers::FObjectFinder<UBlueprint>
		CreepHealthBarFinder(TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/CreepHealthBarWidget_BP.CreepHealthBarWidget_BP'"));
	if (CreepHealthBarFinder.Object != nullptr)
	{
		CreepHealthBarWidgetClass = Cast<UClass>(CreepHealthBarFinder.Object->GeneratedClass);
		widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
		widgetComponent->SetWidgetClass(CreepHealthBarWidgetClass);
		widgetComponent->SetSimulatePhysics(false);
		widgetComponent->bGenerateOverlapEvents = false;
		widgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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

	patrolRadius = 2000.0f;
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;

	//movementComponent->MaxSpeed = 150.0f;
	BaseTurnRate = 45.f;

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

	//FVector pos = AiController->GetBlackboardComponent()->GetValueAsVector("PatrolPosition");
	//FRotator rot = pos.Rotation();
	//FaceRotation(rot);

	//rot.Yaw = FRotator::ClampAxis(rot.Yaw);

	//FaceRotation(rot, DeltaTime);

}

void ACreep::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ACreep::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACreep::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ACreep::TurnAtRate);

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

void ACreep::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACreep::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACreep::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

