// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "PlayerHud.h"
#include "Creep.h"
#include "HeroStats.h"
#include "RespawnOverTime.h"
#include "PlayerCompassWidget.h"
#include "HeroBase.h"


// Sets default values
AHeroBase::AHeroBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//defaults unless set in blueprint
	currentLevel = 1;
	basicAttackDamage = 10.0f;
	respawnTime = 1.0f;

	agroRadius = 800.0f;

	currentArmySize = 0;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//dust particle system class 
	//static ConstructorHelpers::FObjectFinder<AActor> dustPS(TEXT("ParticleSystem'/Game/ParticleEffects/Stomp_Smoke.Stomp_Smoke'"));

	//create & Set sphere trigger for capturing the camp
	sphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerCollider"));
	sphereTrigger->SetSphereRadius(agroRadius, true);
	sphereTrigger->bGenerateOverlapEvents = true;
	sphereTrigger->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	const ConstructorHelpers::FObjectFinder<UClass>
		compassWidgetFinder(TEXT("/Game/UI/PlayerCompassWidget_BP.PlayerCompassWidget_BP_C"));
	if (compassWidgetFinder.Object != NULL)
	{
		CompassWidgetClass = compassWidgetFinder.Object;
		widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
		widgetComponent->SetWidgetClass(CompassWidgetClass);
		widgetComponent->SetSimulatePhysics(false);
		widgetComponent->bGenerateOverlapEvents = false;
		widgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	
	
	
}

// Called when the game starts or when spawned
void AHeroBase::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHeroBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHeroBase::OnOverlapEnd);
	
	GetWorld()->GetAuthGameMode()->Children.Add(this);
	respawnEffect = GetWorld()->SpawnActor<ARespawnOverTime>(respawnClass, FVector::ZeroVector, FRotator::ZeroRotator);
	startingLocation = GetActorLocation();

	if (Cast<UPlayerCompassWidget>(widgetComponent->GetUserWidgetObject()))
	{
		UPlayerCompassWidget* thisPlayerCompassWidget = Cast<UPlayerCompassWidget>(widgetComponent->GetUserWidgetObject());
		thisPlayerCompassWidget->SetOwningHero(this);
	}
	heroStats = new HeroStats(this);
	heroStats->DisplayStats();
	LinkToCreepCamps();
}

// Called every frame
void AHeroBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AHeroBase::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AHeroBase::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHeroBase::MoveRight);

	//Attack
	InputComponent->BindAction("BasicAttack", IE_Pressed, this, &AHeroBase::StartAttack);
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AHeroBase::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AHeroBase::LookUpAtRate);

	InputComponent->BindAxis("CameraZoom", this, &AHeroBase::AdjustCameraZoom);

	InputComponent->BindAction("RecruitCreep", IE_Pressed, this, &AHeroBase::RecruitCreep);

	InputComponent->BindAction("AICamera", IE_Pressed, this, &AHeroBase::SwapAICamera);
}
void AHeroBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHeroBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHeroBase::MoveForward(float Value)
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

void AHeroBase::MoveRight(float Value)
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

void AHeroBase::ShowCampProgress(ACreepCamp* CurrentCamp)
{
	APlayerHUD* playerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	playerHUD->ShowCampProgress(CurrentCamp);
}

void AHeroBase::HideCampProgress()
{
	APlayerHUD* playerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	playerHUD->HideCampProgress();
}


bool AHeroBase::CheckForNearbyEnemyCreeps() 
{

	
	FCollisionObjectQueryParams obejctQP;

	obejctQP.AddObjectTypesToQuery(Creeps);
	
	//Overlap multi by channel as a sphere (for pick ups?)
	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	QueryParameters.OwnerTag = TEXT("Player");


	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,
		GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(600.f),
		QueryParameters);


	nearbyEnemyCreeps.Empty();
	if (Results.Num() > 0)
	{
		
		for (int32 i = 0; i < Results.Num(); i++) 
		{
			nearbyEnemyCreeps.Add(Cast<ACreep>(Results[i].GetActor()));
		}

	}
	return Results.Num() > 0;
	
		
}
bool AHeroBase::CheckForNearbyEnemyHero()
{
	
	FCollisionObjectQueryParams obejctQP;

	obejctQP.AddObjectTypesToQuery(AIHero);

	//Overlap multi by channel as a sphere (for pick ups?)
	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	QueryParameters.OwnerTag = TEXT("Player");


	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,
		GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(300.f),
		QueryParameters);



	if (Results.Num() == 1)
	{
		nearbyEnemyHero = Cast<AHeroBase>(Results[0].GetActor());

	}
	return Results.Num() > 0;
}








void AHeroBase::StartAttack()
{
	UE_LOG(LogTemp, Display, TEXT("Basic Attack PRESSED"));

	AActor *closestEnemy;

	FCollisionObjectQueryParams obejctQP;
	obejctQP.AddObjectTypesToQuery(AIHero);
	obejctQP.AddObjectTypesToQuery(Creeps);
	obejctQP.AddObjectTypesToQuery(DamageableStructures);
	//Overlap multi by channel as a sphere (for pick ups?)
	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	QueryParameters.OwnerTag = TEXT("Player");


	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,
		GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(300.f),
		QueryParameters);

	if (Results.Num() == 0) {
		UE_LOG(LogTemp, Display, TEXT("No Enemies Nearby"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Enemy Found"));
		closestEnemy = Results[0].GetActor();
		for (int i = 0; i < Results.Num(); i++)
		{
			if (GetDistanceTo(Results[i].GetActor()) <= GetDistanceTo(closestEnemy))
			{
				closestEnemy = Results[i].GetActor();
			}

		}
		//check if they are on the same team as us or not
		if (!closestEnemy->ActorHasTag(team))
		{
			Attack(closestEnemy);
		}
		
	}
}
void AHeroBase::ResetHealth()
{
	currentHealth = maxHealth;
}
void AHeroBase::Attack(AActor* enemy)
{
	FDamageEvent DamageEvent;

	float damage = enemy->TakeDamage(basicAttackDamage, DamageEvent, Cast<APlayerController>(GetController()), this);
}

void AHeroBase::AdjustCameraZoom(float Value)
{
	
	if (Value < 0 && FollowCamera->FieldOfView >= 90)
	{
		UE_LOG(LogTemp, Display, TEXT("Zooming Camera Down"));
		GetCameraBoom()->TargetArmLength += Value * 10.0f;
	}
	else if (Value> 0 && FollowCamera->FieldOfView <= 120)
	{
		UE_LOG(LogTemp, Display, TEXT("Zooming Camera UP"));
		GetCameraBoom()->TargetArmLength += Value * 10.0f;
	}
}

void AHeroBase::OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (Cast<ACreepCamp>(OtherActor))
	{
		visitingCamp = Cast<ACreepCamp>(OtherActor);
	}
}

void AHeroBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACreepCamp>(OtherActor))
	{
		visitingCamp = nullptr;
	}
}

float AHeroBase::GetPlayerHealthPercentage()
{
	return currentHealth / maxHealth;
}

float AHeroBase::GetCurrentHealth()
{
	return currentHealth;
}

float AHeroBase::GetMaxHealth()
{
	return maxHealth;
}

void AHeroBase::LevelUp()
{
	currentLevel++;

	maxHealth += healthIncreasePerLevel;
	currentHealth = maxHealth;

	basicAttackDamage += damageIncreasePerLevel;
}

float AHeroBase::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) 
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	currentHealth -= DamageAmount;

	UE_LOG(LogTemp, Log, TEXT("Hero took %f damage."), DamageAmount);
	if (currentHealth <= 0 && !bIsRespawning)
	{
		currentHealth = 0;
		GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		GetMesh()->SetVisibility(false);
		SetActorEnableCollision(false);
		bIsRespawning = true;
		respawnEffect->StartTimer(respawnTime, this);
		
	}
	return DamageAmount;

}

void AHeroBase::RecruitCreep()
{
	UE_LOG(LogTemp, Warning, TEXT("Recruit Creep!"));
	if (visitingCamp)
	{
		UE_LOG(LogTemp, Warning, TEXT("At Camp!"));
		if (currentArmySize < maxArmySize)
		{
			ACreep* creep = visitingCamp->SendCreepToPlayer(this);
			if (creep)
			{
				//IMPLEMENT SOUND TO PLAY!
				currentArmySize++;
				CreepArmy.Add(creep);
				creep->JoinPlayerArmy(this);
				UE_LOG(LogTemp, Warning, TEXT("Creep successfully recruited!"));
			}
		}
		else
		{
		    //DISPLAY MESSAGE TO PLAYER THAT HE IS AT HIS MAX CREEP ARMY SIZE!
			UE_LOG(LogTemp, Warning, TEXT("At max creep army size!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not near a camp to recruit a creep!"));
	}
	
}

void AHeroBase::SwapAICamera()
{
	APlayerController* OurPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (OurPlayerController->GetViewTarget() == this)
	{
		if (AICam != nullptr)
			OurPlayerController->SetViewTargetWithBlend(AICam);
	}
	else
	{
		OurPlayerController->SetViewTargetWithBlend(this);
	}
}
void AHeroBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	delete heroStats;
}

void AHeroBase::LinkToCreepCamps() 
{
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), creepCampClass, actorList);
	TArray<ACreepCamp*> creepCamps;
	for (int32 i = 0; i < actorList.Num(); i++)
	{
		creepCamps.Add(Cast<ACreepCamp>(actorList[i]));
		creepCamps[i]->LinkToHeroes(this);
	}

}

void AHeroBase::AddToCapturedCamps(ACreepCamp* camp) 
{
	if (!capturedCamps.Contains(camp)) 
	{
		capturedCamps.Add(camp);
	}
}

void AHeroBase::RemoveFromCapturedCamps(ACreepCamp* camp) 
{
	if (capturedCamps.Contains(camp))
	{
		capturedCamps.Remove(camp);
	}
}

void AHeroBase::UpdateHeroStats() 
{
	heroStats->UpdateStats();
	heroStats->DisplayStats();

}