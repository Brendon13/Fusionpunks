// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Fusionpunks.h"
#include "TestCreep.h"
#include "MobaCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AMobaCharacter

AMobaCharacter::AMobaCharacter()
{
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
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	


}

//////////////////////////////////////////////////////////////////////////
// Input

void AMobaCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("Basic Attack", IE_Pressed, this, &AMobaCharacter::DetermineClickEvent);
	InputComponent->BindAction("Skill1", IE_Pressed, this, &AMobaCharacter::OnSkillPressed);

	InputComponent->BindAxis("MoveForward", this, &AMobaCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMobaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AMobaCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AMobaCharacter::LookUpAtRate);
	InputComponent->BindAxis("CameraZoom", this, &AMobaCharacter::AdjustCameraZoom);
	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AMobaCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AMobaCharacter::TouchStopped);
}


void AMobaCharacter::DetermineClickEvent() 
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

void AMobaCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AMobaCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AMobaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMobaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMobaCharacter::MoveForward(float Value)
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
void AMobaCharacter::AdjustCameraZoom(float Value) 
{
	
	
	
	if (Value < 0 && FollowCamera->FieldOfView >= 90)
	{
		UE_LOG(LogTemp, Display, TEXT("Zooming Camera Down"));
		FollowCamera->FieldOfView += Value* 5.0f;
	}
	else if (Value> 0 && FollowCamera->FieldOfView <= 120)
	{
		UE_LOG(LogTemp, Display, TEXT("Zooming Camera UP"));
		FollowCamera->FieldOfView += Value* 5.0f;
	}
}
void AMobaCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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
void AMobaCharacter::BeginPlay(){
	Super::BeginPlay();
	maxHp = 250;
	currHP = maxHp;

}

void AMobaCharacter::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	if (skillSelected) 
	{
		UpdateTarget();
	}
}


float AMobaCharacter::GetHpPercent() {
	percentHP = currHP / maxHp;
	return percentHP;
}

float AMobaCharacter::GetCurrHP() {
	return currHP;
}
float AMobaCharacter::GetMaxHP() {
	return maxHp;
}

void AMobaCharacter::ApplyDamage(float dmg) {
	currHP -= dmg;
	
}

void AMobaCharacter::Attack(AActor* enemy) 
{
	
	FDamageEvent DamageEvent;
	
	float damage = enemy->TakeDamage(20.0f, DamageEvent,Cast<APlayerController>(GetController()), this);

	
}

void AMobaCharacter::StartAttack()
{
	UE_LOG(LogTemp, Display, TEXT("Basic Attack PRESSED"));

	AActor *closestEnemy;

	FCollisionObjectQueryParams obejctQP;

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
		closestEnemy= Results[0].GetActor();
		for (int i = 0; i < Results.Num(); i++)
		{
			if (GetDistanceTo(Results[i].GetActor()) <= GetDistanceTo(closestEnemy))
			{
				closestEnemy = Results[i].GetActor();
			}

		}
		Attack(closestEnemy);
	}

	
}

AActor* AMobaCharacter::UpdateTarget()
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
	if (skillTargetResults.Num() == 0) {
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

void AMobaCharacter::OnSkillPressed()
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

void AMobaCharacter::UseSkill(AActor* enemy) 
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

void AMobaCharacter::AddAffectedActor(AActor* enemy) 
{
	affectedActors.Add(enemy);
}
bool AMobaCharacter::IsAffected(AActor* enemy) 
{
	return affectedActors.Contains(enemy);
}

void AMobaCharacter::UnHighlightTarget(AActor* enemy)
{
	if (enemy->IsA(ATestCreep::StaticClass()))
	{
		Cast<ATestCreep>(enemy)->GetMesh()->SetRenderCustomDepth(false);
		
	}

}

void AMobaCharacter::HighlightTarget(AActor* enemy, TArray<FOverlapResult> enemies)
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

void AMobaCharacter::UnHighlightAll(TArray<FOverlapResult> enemies)
{
	for (int i = 0; i < enemies.Num(); i++)
	{
		if (enemies[i].GetActor()->IsA(ATestCreep::StaticClass()))
		{
			Cast<ATestCreep>(enemies[i].GetActor())->GetMesh()->SetRenderCustomDepth(false);
		}

	}

}

