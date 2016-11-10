// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "CreepCamp.h"
#include "PlayerHud.h"
#include "Creep.h"
#include "DieselTower.h"
#include "HeroStats.h"
#include "Base.h"
#include "HeroAIController.h"
#include "RespawnOverTime.h"
#include "PlayerCompassWidget.h"
#include "CreepFormation.h"
#include "HealOverTime.h"
#include "AbilityBase.h"
#include "FusionpunksGameState.h"
#include "BulletBase.h"
#include "HeroBase.h"


// Sets default values
AHeroBase::AHeroBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.0f;
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

	/*const ConstructorHelpers::FObjectFinder<UClass>
		compassWidgetFinder(TEXT("/Game/UI/PlayerCompassWidget_BP.PlayerCompassWidget_BP_C"));
	if (compassWidgetFinder.Object != NULL)
	{
		CompassWidgetClass = compassWidgetFinder.Object;
		widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
		widgetComponent->SetWidgetClass(CompassWidgetClass);
		widgetComponent->SetSimulatePhysics(false);
		widgetComponent->bGenerateOverlapEvents = false;
		widgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}*/

	creepFormationComp = CreateDefaultSubobject<UCreepFormation>(TEXT("CreepFormationComponent"));
	//commandAttackCount = 0;

	compassDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("CompassDecalComponent"));
	compassDecalComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	AbilitiesClass.SetNum(NUMBEROFABILITIES);
	Abilities.SetNum(NUMBEROFABILITIES);

	bIsAttacking = false; 

	currentExperience = 0;
	currentHealth = maxHealth;

}

// Called when the game starts or when spawned
void AHeroBase::BeginPlay()
{
	Super::BeginPlay();

	currentHealth = maxHealth;
	DefaultTurnRate = BaseTurnRate;
	DefaultMaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if (GetWorld())
	{
		AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
		if (gameState)
		{
			gameState->Players.Add(this);
		}
	}

	compassDecalMaterialDynamic = compassDecalComponent->CreateDynamicMaterialInstance();

	FLinearColor materialColor = FLinearColor::FLinearColor(1 - GetPlayerHealthAsDecimal(), GetPlayerHealthAsDecimal(), 0, 1.0f);
	compassDecalMaterialDynamic->SetVectorParameterValue("Base_Colour", materialColor);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AHeroBase::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AHeroBase::OnOverlapEnd);
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AHeroBase::OnCapsuleComponentHit);
	
	GetWorld()->GetAuthGameMode()->Children.Add(this);
	respawnEffect = GetWorld()->SpawnActor<ARespawnOverTime>(respawnClass, FVector::ZeroVector, FRotator::ZeroRotator);

	startingLocation = GetActorLocation();
	
	/*if (Cast<UPlayerCompassWidget>(widgetComponent->GetUserWidgetObject()))
	{
		UPlayerCompassWidget* thisPlayerCompassWidget = Cast<UPlayerCompassWidget>(widgetComponent->GetUserWidgetObject());
		thisPlayerCompassWidget->SetOwningHero(this);
	}*/
	heroStats = new HeroStats(this);
	heroStats->DisplayStats();
	LinkToCreepCamps();

	if (ActorHasTag("AI")) 
	{
		heroAI = Cast<AHeroAIController>(GetController());
	}

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	//Abilities[1] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[1], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	//Abilities[2] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[2], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	if(AbilitiesClass[3]!= nullptr)
		Abilities[3] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[3], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	//Abilities[4] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[4], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
	
}

// Called every frame
void AHeroBase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/*if (Tags.Contains("Cyber"))
	{
		UE_LOG(LogTemp, Warning, TEXT("current experience is: %i"), currentExperience);
	}*/
	//UMaterialInstance* material = Cast<UMaterialInstance>(compassDecalComponent->GetDecalMaterial());
	//if (material)
	//{
	//	material->Para
	//	if (material->VectorParameterValues.Num() > 0)
	//	{
	//		material->VectorParameterValues[0].ParameterValue.R = 1 - GetPlayerHealthAsDecimal();
	//		material->VectorParameterValues[0].ParameterValue.G = GetPlayerHealthAsDecimal();
	//		material->VectorParameterValues[0].ParameterValue.B = 0;
	//			//FLinearColor(1 - GetPlayerHealthAsDecimal(), GetPlayerHealthAsDecimal(), 0, 1.0f);
	//	}
	//}

	if (ActorHasTag("AI"))
	{
		if (bCreepIsAttacking && attackingCreep->GetBIsDead() || attackingCreep == nullptr)
		{
			attackingCreep = nullptr;
			bCreepIsAttacking = false;

		}
	}
}

// Called to bind functionality to input
void AHeroBase::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	// Set up gameplay key bindings
	check(InputComponent);

	//InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AHeroBase::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHeroBase::MoveRight);

	//Attack
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AHeroBase::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AHeroBase::LookUpAtRate);

	InputComponent->BindAxis("CameraZoom", this, &AHeroBase::AdjustCameraZoom);

	InputComponent->BindAction("RecruitCreep", IE_Pressed, this, &AHeroBase::RecruitCreep);
	//InputComponent->BindAction("CreepCommandAttack", IE_Pressed, this, &AHeroBase::CreepCommand_AttackTarget);

	InputComponent->BindAction("AICamera", IE_Pressed, this, &AHeroBase::SwapAICamera);

	//InputComponent->BindAction("SacrificeCreep", IE_Pressed, this, &AHeroBase::SacrificeCreep);

	InputComponent->BindAction("Ability1", IE_Pressed, this, &AHeroBase::UseAbility0);
	InputComponent->BindAction("Ability2", IE_Pressed, this, &AHeroBase::UseAbility1);
	InputComponent->BindAction("Ability3", IE_Pressed, this, &AHeroBase::UseAbility2);
	InputComponent->BindAction("Ability4", IE_Pressed, this, &AHeroBase::UseAbility3);
	InputComponent->BindAction("Ability5", IE_Pressed, this, &AHeroBase::UseAbility4);

	//InputComponent->BindAction("RangedAttack", IE_Pressed, this, &AHeroBase::RangedAttack);
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
		FCollisionShape::MakeSphere(1000.f),
		QueryParameters);

	nearbyEnemyCreeps.Empty();
	if (Results.Num() > 0)
	{
		for (int32 i = 0; i < Results.Num(); i++) 
		{
			if (team.Compare("Diesel") == 0 && !Results[i].GetActor()->ActorHasTag("Diesel"))
			{
				nearbyEnemyCreeps.Add(Cast<ACreep>(Results[i].GetActor()));
			}

			else if (team.Compare("Cyber") == 0 && !Results[i].GetActor()->ActorHasTag("Cyber"))
			{
				nearbyEnemyCreeps.Add(Cast<ACreep>(Results[i].GetActor()));
			}
		}

	}
	return nearbyEnemyCreeps.Num() > 0;	
}
bool AHeroBase::CheckForNearbyEnemyHero()
{
	FCollisionObjectQueryParams obejctQP;

	obejctQP.AddObjectTypesToQuery(Hero);

	//Overlap multi by channel as a sphere (for pick ups?)
	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	QueryParameters.OwnerTag = TEXT("Player");

	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,
		GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(1300),
		QueryParameters);
		
	nearbyEnemyHero = nullptr;

	if (Results.Num() == 1)
	{
		nearbyEnemyHero = Cast<AHeroBase>(Results[0].GetActor());

	}
	return nearbyEnemyHero != nullptr;
}

bool AHeroBase::CheckForNearbyInteractions()
{
	FCollisionObjectQueryParams obejctQP;

	obejctQP.AddObjectTypesToQuery(Hero);
	obejctQP.AddObjectTypesToQuery(CreepCampTrigger);
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
		FCollisionShape::MakeSphere(1000.f),
		QueryParameters);

	nearbyEnemyCreeps.Empty();
	nearbyOwnedCreepCamps.Empty();
	nearbyEnemyHero = nullptr;
	nearbyEnemyCamp = nullptr;
	if (Results.Num() > 0)
	{
		for (int32 i = 0; i < Results.Num(); i++)
		{

			if (Results[i].GetActor()->IsA(AHeroBase::StaticClass()))
			{
				nearbyEnemyHero = Cast<AHeroBase>(Results[i].GetActor());
			}
					
			else if (Results[i].GetActor()->IsA(ACreepCamp::StaticClass()))
			{

				ACreepCamp* currCamp = Cast<ACreepCamp>(Results[i].GetActor());
				if (team.Compare("Diesel") == 0 && currCamp->GetCampType() == ECampType::CT_Diesel)
				{
					nearbyOwnedCreepCamps.Add(currCamp);
				}

				else if (team.Compare("Cyber") == 0 && currCamp->GetCampType() == ECampType::CT_Cyber)
				{
					nearbyOwnedCreepCamps.Add(currCamp);
				}

				else
				{
					nearbyEnemyCamp = currCamp;
				}
			}

			else if (Results[i].GetActor()->IsA(ACreep::StaticClass()))
			{


				if (team.Compare("Diesel") == 0 && !Results[i].GetActor()->ActorHasTag("Diesel"))
				{
					nearbyEnemyCreeps.Add(Cast<ACreep>(Results[i].GetActor()));
				}

				else if (team.Compare("Cyber") == 0 && !Results[i].GetActor()->ActorHasTag("Cyber"))
				{
					nearbyEnemyCreeps.Add(Cast<ACreep>(Results[i].GetActor()));
				}
			}


		}

	}
	return nearbyOwnedCreepCamps.Num() > 0 || nearbyEnemyHero != nullptr  || nearbyEnemyCreeps.Num() > 0 || nearbyEnemyCamp != nullptr;
}

bool AHeroBase::CheckForNearbyOnwedCreepCamps()
{
	FCollisionObjectQueryParams obejctQP;
	obejctQP.AddObjectTypesToQuery(CreepCampTrigger);	
	//Overlap multi by channel as a sphere (for pick ups?)
	FCollisionQueryParams QueryParameters;
	QueryParameters.AddIgnoredActor(this);
	QueryParameters.OwnerTag = TEXT("Player");

	TArray<FOverlapResult> Results;
	GetWorld()->OverlapMultiByObjectType(Results,
		GetActorLocation(),
		FQuat(),
		obejctQP,
		FCollisionShape::MakeSphere(1000.f),
		QueryParameters);

	nearbyOwnedCreepCamps.Empty();	
	if (Results.Num() > 0)
	{
		for (int32 i = 0; i < Results.Num(); i++)
		{
	
			ACreepCamp* currCamp = Cast<ACreepCamp>(Results[i].GetActor());
			if (team.Compare("Diesel") == 0 && currCamp->GetCampType() == ECampType::CT_Diesel)
			{
				nearbyOwnedCreepCamps.Add(currCamp);
			}

			else if (team.Compare("Cyber") == 0 && currCamp->GetCampType() == ECampType::CT_Cyber)
			{
				nearbyOwnedCreepCamps.Add(currCamp);
			}
			
			
		}

	}

	return nearbyOwnedCreepCamps.Num() > 0;
}

void AHeroBase::StartAttack()
{
	//UE_LOG(LogTemp, Display, TEXT("Basic Attack PRESSED"));

	AActor *closestEnemy;

	FCollisionObjectQueryParams obejctQP;
	obejctQP.AddObjectTypesToQuery(Hero);
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

	//UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("MeleeAttacking"));
	//if (boolProp)
	//{
	//	bIsAttacking = true;
	//	boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), true);
	//	//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
	//	GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AHeroBase::StopAttacking, 1.0f, false);
	//}
}

void AHeroBase::SetIsCapturing(bool status, class ACreepCamp* camp)
 {
 	if (status)
 	{
 		campBeingCaptured = camp;
 		
 
 	}
 	else
 	{
 		campBeingCaptured = nullptr;
 
 	}
 	isCapturing = status;
 
 
 }

void AHeroBase::ResetHealth()
{
	currentHealth = maxHealth;
	FLinearColor materialColor = FLinearColor::FLinearColor(1 - GetPlayerHealthAsDecimal(), GetPlayerHealthAsDecimal(), 0, 1.0f);
	compassDecalMaterialDynamic->SetVectorParameterValue("Base_Colour", materialColor);
}
void AHeroBase::Attack(AActor* enemy)
{
	FDamageEvent DamageEvent;
	
	float damage = enemy->TakeDamage(basicAttackDamage, DamageEvent,GetController(), this);
}

void AHeroBase::AdjustCameraZoom(float Value)
{
	
	if (Value < 0 && FollowCamera->FieldOfView >= 90)
	{
		//UE_LOG(LogTemp, Display, TEXT("Zooming Camera Down"));
		GetCameraBoom()->TargetArmLength += Value * 10.0f;
	}
	else if (Value> 0 && FollowCamera->FieldOfView <= 120)
	{
		//UE_LOG(LogTemp, Display, TEXT("Zooming Camera UP"));
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

void AHeroBase::OnCapsuleComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hero was hit by something"));
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

	//FOR AI HERO
	if (ActorHasTag("AI") && DamageCauser->IsA(AHeroBase::StaticClass()))
		bHeroIsAttacking = true;

	else if (ActorHasTag("AI") && DamageCauser->IsA(ACreep::StaticClass()) && !bCreepIsAttacking)
	{
		bCreepIsAttacking = true;
		attackingCreep = Cast<ACreep>(DamageCauser);
	}


	if (!DamageCauser->ActorHasTag("AI") && !DamageCauser->ActorHasTag("Creep") && FloatingDamageWidgetClass)
	{
		UFloatingDamageWidget* floatingDamageWidget = CreateWidget<UFloatingDamageWidget>(GetWorld()->GetFirstPlayerController(), FloatingDamageWidgetClass);
		floatingDamageWidget->SetAlignmentInViewport(FVector2D::FVector2D(0.5f, 0.5f));
		floatingDamageWidget->SetIncDamage(DamageAmount);
		floatingDamageWidget->SetOwningActor(this);
		floatingDamageWidget->AddToViewport();
	}

	FLinearColor materialColor = FLinearColor::FLinearColor(1 - GetPlayerHealthAsDecimal(), GetPlayerHealthAsDecimal(), 0, 1.0f);
	compassDecalMaterialDynamic->SetVectorParameterValue("Base_Colour", materialColor);

	if (currentHealth <= 0 && !bIsRespawning)
	{
		if (DamageCauser->IsA(ADieselTower::StaticClass()))
		{
			Cast<ADieselTower>(DamageCauser)->RemoveFromTargetList(this);
		}

		AHeroBase* hero = Cast<AHeroBase>(DamageCauser);
		if (hero)
		{
			UE_LOG(LogTemp, Log, TEXT("%i experence rewarded"), XPKillReward);
			hero->AddToExperience(XPKillReward);
		}

		currentHealth = 0;
		GetController()->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		GetMesh()->SetVisibility(false);
		SetActorEnableCollision(false);
		bIsRespawning = true;
		respawnEffect->StartTimer(respawnTime, this);
		if (ActorHasTag("AI")){
			
			heroAI->ResetAITreeTaskStatus();
			heroAI->RestartHeroAITree();
		}

	}
	return DamageAmount;

}

void AHeroBase::RecruitCreep()
{
	if (visitingCamp)
	{
		if (currentArmySize < maxArmySize)
		{
			ACreep* creep = visitingCamp->GetNextCreep(this);
			if (creep)
			{
				//IMPLEMENT SOUND TO PLAY!
				currentArmySize++;
				CreepArmy.Add(creep);
				creepFormationComp->CalculateSlotPositions(currentArmySize);
				creep->JoinPlayerArmy(this, currentArmySize);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Cannot recruit creeps from the opposing teams!"));
				return;
			}
		}
		else
		{
		    //DISPLAY MESSAGE TO PLAYER THAT HE IS AT HIS MAX CREEP ARMY SIZE!
			UE_LOG(LogTemp, Warning, TEXT("At max creep army size!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not near a camp to recruit a creep!"));
	}
}

void AHeroBase::RemoveCreepFromArmy(ACreep* creep)
{
	if (CreepArmy.Contains(creep))
	{
		CreepArmy.Remove(creep);
		currentArmySize--;
		creepFormationComp->CalculateSlotPositions(currentArmySize);
		UpdateCreepArmy();
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
		isCapturing = false;
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


TArray<ACreep*> AHeroBase::GetCreepArmyArray()
{
	return CreepArmy;
}

void AHeroBase::UpdateCreepArmy()
{
	for (int32 i = 1; i <= CreepArmy.Num(); i++)
	{
		if (CreepArmy[i - 1] != nullptr)
		{
			CreepArmy[i - 1]->slotAssignment = i;
		}	
	}
}

FVector AHeroBase::GetSlotPosition(int SlotNumber)
{
	FVector pos = creepFormationComp->GetPositionInFormation(SlotNumber);
	pos += GetActorLocation();
	return pos; 
}

float AHeroBase::GetMaxHealth() 
{
	return maxHealth;
}

//NOTE::Brendon -  Add variable for the length of the raycast to adjust in blueprint
//NOTE::Brendon - Change FCollisionShape::MakeSphere to a raycast that is dependent on a normalized vector direction 
//AActor* AHeroBase::CreepCommand_Attack_CheckTarget(FVector Direction)
//{
//	FVector direction = Direction;
//
//	FCollisionObjectQueryParams obejctQP;
//	obejctQP.AddObjectTypesToQuery(Creeps);
//
//	FCollisionQueryParams QueryParameters;
//	QueryParameters.AddIgnoredActor(this);
//	QueryParameters.OwnerTag = TEXT("Player");
//	GetWorld()->OverlapMultiByObjectType(creepCommand_TargetResults, direction, FQuat(), obejctQP, FCollisionShape::MakeSphere(1000.0f), QueryParameters);
//
//	AActor* closestEnemy = nullptr;
//	if (creepCommand_TargetResults.Num() > 0)
//	{
//		closestEnemy = creepCommand_TargetResults[0].GetActor();
//
//		for (int i = 0; i < creepCommand_TargetResults.Num(); i++)
//		{
//			if (GetDistanceTo(creepCommand_TargetResults[i].GetActor()) <= GetDistanceTo(closestEnemy))
//			{
//				closestEnemy = creepCommand_TargetResults[i].GetActor();
//			}
//		}
//	}
//	return closestEnemy;
//}
//
//void AHeroBase::CreepCommand_AttackTarget()
//{
//	AActor* EnemyTarget = CreepCommand_Attack_CheckTarget(GetActorLocation());
//	if (EnemyTarget)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Enemy Found! (CreepCommand_AttackTarget)"));
//		HighlightCreepArmyTarget(EnemyTarget, creepCommand_TargetResults);
//
//		for (int i = 0; i < CreepArmy.Num(); i++)
//		{
//			CreepArmy[i]->SetEnemyTarget(EnemyTarget);
//		}
//	}
//
//	/*UE_LOG(LogTemp, Warning, TEXT("Initiate CreepCommand_AttackTarget"));
//	AActor* EnemyTarget = CreepCommand_Attack_CheckTarget(GetActorLocation());
//	if (commandAttackCount == 0)
//	{
//		if (EnemyTarget)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Enemy Found! (CreepCommand_AttackTarget)"));
//			HighlightCreepArmyTarget(EnemyTarget, creepCommand_TargetResults);
//			commandAttackCount++;
//		}
//		else
//		{
//			UE_LOG(LogTemp, Warning, TEXT("No Enemy Target Found! (CreepCommand_AttackTarget)"));
//			commandAttackCount = 0;
//		}
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Setting Enemy (CreepCommand_AttackTarget)"));
//		commandAttackCount = 0;
//		for (int i = 0; i < CreepArmy.Num(); i++)
//		{
//			CreepArmy[i]->SetEnemyTarget(EnemyTarget);
//		}
//	}*/
//}
//
//void AHeroBase::UnHighlightCreepArmyTarget(AActor* enemy)
//{
//	if (enemy->IsA(ACreep::StaticClass()))
//	{
//		Cast<ACreep>(enemy)->GetMesh()->SetRenderCustomDepth(false);
//	}
//}
//
//void AHeroBase::HighlightCreepArmyTarget(AActor* enemy, TArray<FOverlapResult> enemies)
//{
//	if (enemy->IsA(ACreep::StaticClass()))
//	{
//		Cast<ACreep>(enemy)->GetMesh()->SetRenderCustomDepth(true);
//		Cast<ACreep>(enemy)->GetMesh()->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
//	}
//
//	for (int i = 0; i < enemies.Num(); i++)
//	{
//		if (enemy != enemies[i].GetActor() && enemies[i].GetActor()->IsA(ACreep::StaticClass()))
//		{
//			Cast<ACreep>(enemies[i].GetActor())->GetMesh()->SetRenderCustomDepth(false);
//		}
//
//	}
//}

void AHeroBase::HealOverTime()
{
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnParameters.Owner = this; 
	AHealOverTime* healOverTime = GetWorld()->SpawnActor<AHealOverTime>(healOverTimeClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParameters);
	healOverTime->SetTotalHealthValue(maxHealth * 0.5f, 0.005f);
	healOverTime->StartTimer(0.1f, this);
}

void AHeroBase::Heal(float amount)
{
	currentHealth += amount;
	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}
}

bool AHeroBase::SacrificeCreep()
{
	//UE_LOG(LogTemp, Warning, TEXT("Current health: %f"), currentHealth);
	if (CreepArmy.Num() > 0 && currentHealth < maxHealth)
	{
		ACreep* creep = CreepArmy.Pop();
		creep->TakeDamage(10000000, FDamageEvent::FDamageEvent(), GetController(), this);
	    UE_LOG(LogTemp, Warning, TEXT("Sacrificed Creep...From herobase"));
		HealOverTime();
		return true;
	}
	return false;
}

void AHeroBase::UseAbility0()
{
	UE_LOG(LogTemp, Warning, TEXT("Using Ability 0"));
	if (AbilitiesClass[0] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Ability 0"));
		//spawn the ability
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		if (Abilities[0] == nullptr)
		{
			Abilities[0] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[0], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			Abilities[0]->Use();
			return;
		}
		Abilities[0]->Use();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability 0 is Unassigned!"))
	}
}

void AHeroBase::UseAbility1()
{

	UE_LOG(LogTemp, Warning, TEXT("Using Ability 1"));
	if (AbilitiesClass[1] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Ability 1"));
		//spawn the ability
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		if (Abilities[1] == nullptr)
		{
			Abilities[1] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[1], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			Abilities[1]->Use();
			return;
		}
		Abilities[1]->Use();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability 1 is Unassigned!"))
	}
}

void AHeroBase::UseAbility2()
{
	UE_LOG(LogTemp, Warning, TEXT("Using Ability 2"));
	if (AbilitiesClass[2] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Ability 2"));
		//spawn the ability
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		if (Abilities[2] == nullptr)
		{
			Abilities[2] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[2], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			Abilities[2]->Use();
			return;
		}
		Abilities[2]->Use();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability 2 is Unassigned!"))
	}
}

void AHeroBase::UseAbility3()
{
	UE_LOG(LogTemp, Warning, TEXT("Using Ability 3"));
	if (AbilitiesClass[3] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Ability 3"));
		//spawn the ability
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		if (Abilities[3] == nullptr)
		{
			Abilities[3] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[3], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			Abilities[3]->Use();
			return;
		}
		Abilities[3]->Use();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability 3 is Unassigned!"))
	}
}

void AHeroBase::UseAbility4()
{
	UE_LOG(LogTemp, Warning, TEXT("Using Ability 4"));
	if (AbilitiesClass[4] != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawning Ability 4"));
		//spawn the ability
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;

		if (Abilities[4] == nullptr)
		{
			Abilities[4] = GetWorld()->SpawnActor<AAbilityBase>(AbilitiesClass[4], GetActorLocation(), FRotator::ZeroRotator, spawnParams);
			Abilities[4]->Use();
			return;
		}
		Abilities[4]->Use();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability 4 is Unassigned!"))
	}
}

void AHeroBase::StopAttacking()
{
	if (attackTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(attackTimerHandle);
	
	UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("MeleeAttacking"));
	if (boolProp)
	{
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		bIsAttacking = false;
		boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), false);
		//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
	}
}

void AHeroBase::AddToExperience(int experience)
{
	currentExperience += experience;

	if (currentExperience >= experienceGoal)
	{
		currentExperience -= experienceGoal;
		LevelUp();
	}
}

void AHeroBase::CreepArmyChangeTeam(bool Attack)
{
	for (int i = 0; i < CreepArmy.Num(); i++)
	{
		CreepArmy[i]->ChangeTeam();
		
		if (Attack)
		{
			CreepArmy[i]->AttackLeader();
		}
	}
}

void AHeroBase::RestoreTurnRate()
{
	BaseTurnRate = DefaultTurnRate;
}

void AHeroBase::RestoreWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

void AHeroBase::ResetMeleeAttackCombo()
{
	bIsMeleeAttacking = false; 
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkSpeed;
	BaseTurnRate = DefaultTurnRate;
}

void AHeroBase::MeleeAttack()
{
	//bIsAttacking = true; 
	//GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AHeroBase::StopAttacking, 1.25f, false);

	//if (!bIsAttacking)
	//{
	//	UBoolProperty* boolProp = FindField<UBoolProperty>(GetMesh()->GetAnimInstance()->GetClass(), TEXT("MeleeAttacking"));
	//	if (boolProp)
	//	{
	//		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//		bIsAttacking = true;
	//		boolProp->SetPropertyValue_InContainer(GetMesh()->GetAnimInstance(), true);
	//		//bool meleeAttack = boolProp->GetPropertyValue_InContainer(GetMesh()->GetAnimInstance());
	//		GetWorld()->GetTimerManager().SetTimer(attackTimerHandle, this, &AHeroBase::StopAttacking, 1.66f, false);
	//	}
	//}

	//if (!bIsMeleeAttacking)
	//{
	//	bIsMeleeAttacking = true; 
	//	if (MeleeAnimSequenceArray.Num() > 0)
	//	{
	//		//random animation to play 
	//		int random = FMath::RandRange(0, MeleeAnimSequenceArray.Num() - 1);
	//		//MeleeAnimSequenceArray[random]->Play
	//	}
	//}

	////if we are attacking for the first time
	//if (!bIsMeleeAttacking)
	//{
	//	bIsMeleeAttacking = true; 
	//	MeleeCombo = 1; 
	//	if (MeleeAnimSequenceArray.Num() > 0)
	//	{
	//		float length = MeleeAnimSequenceArray[0]->GetPlayLength();
	//		//Disable Turn rate && slow movementspeed
	//		//BaseTurnRate = 0;
	//		GetCharacterMovement()->MaxWalkSpeed = MeleeAttackSlowSpeed;
	//	}
	//	
	//	//Play first animation
	//	//Get length of first animation and pause movement until it has finished playing?
	//}
	////we must be in the middle of a combo
	//else
	//{
	//	switch (MeleeCombo)
	//	{
	//		case 1:
	//		{
	//			MeleeCombo = 2;
	//			if (MeleeAnimSequenceArray.Num() > 1)
	//			{
	//				float length = MeleeAnimSequenceArray[1]->GetPlayLength();
	//				//start timer for hit next combo
	//			}
	//		}break;
	//		case 2:
	//		{
	//			MeleeCombo = 3;
	//			if (MeleeAnimSequenceArray.Num() > 2)
	//			{
	//				float length = MeleeAnimSequenceArray[2]->GetPlayLength();
	//				//start timer for hit next combo
	//			}
	//		}break;
	//		case 3:
	//		{
	//			//hit last combo, reset melee combo
	//			MeleeCombo = 0;
	//			if (MeleeAnimSequenceArray.Num() > 1)
	//			{
	//				float length = MeleeAnimSequenceArray[1]->GetPlayLength();
	//				//start timer for hit next combo
	//			}
	//		}break;
	//			
	//	}

	//}

	
}

void AHeroBase::RangedAttack()
{
	if (BulletClass)
	{
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = this;
		spawnParameters.Instigator = this; 
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<ABulletBase>(BulletClass, GetActorLocation() + GetActorForwardVector() * 100, FRotator::ZeroRotator, spawnParameters);
	}
	
}