// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Fusionpunks.h"
#include "PlayerHUD.h"
#include "FusionpunksCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AFusionpunksCharacter

AFusionpunksCharacter::AFusionpunksCharacter()
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

	maxHealth = 10;
	currentHealth = maxHealth;

	pullRadius = 500.0f;
	pullForce = 200.0f;

	leapUpwardForce = 2000.0f;
	leapDownwardForce = 4000.0f;
	leapMaxAirTime = 0.75f;
	timeInAirTimer = leapMaxAirTime;
	bIsLeaping = false;

	dashForce = 16000.0f;
	dashMaxTime = 0.25f;
	dashTimer = dashMaxTime;
	bIsDashing = false;
}

void AFusionpunksCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetAuthGameMode()->Children.Add(this);
}

void AFusionpunksCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLeaping)
	{
		timeInAirTimer -= DeltaTime;

		if (timeInAirTimer <= 0)
		{
			bIsLeaping = false;
			LaunchCharacter(FVector::UpVector * -1 * leapDownwardForce, true, true);
		}
	}

	if (bIsDashing)
	{
		dashTimer -= DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("Is Dashing!"));
		if (dashTimer <= 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Stop Dashing Force!"));
			bIsDashing = false;
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
void AFusionpunksCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AFusionpunksCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFusionpunksCharacter::MoveRight);

	//Attack
	InputComponent->BindAction("BasicAttack", IE_Pressed, this, &AFusionpunksCharacter::StartAttack);
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AFusionpunksCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AFusionpunksCharacter::LookUpAtRate);

	InputComponent->BindAction(TEXT("Leap"), IE_Pressed, this, &AFusionpunksCharacter::Leap);
	InputComponent->BindAction(TEXT("ForcePull"), IE_Pressed, this, &AFusionpunksCharacter::ForcePull);
	InputComponent->BindAction("Dash", IE_Pressed, this, &AFusionpunksCharacter::Dash);

	InputComponent->BindAxis("CameraZoom", this, &AFusionpunksCharacter::AdjustCameraZoom);

}

void AFusionpunksCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFusionpunksCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFusionpunksCharacter::MoveForward(float Value)
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

void AFusionpunksCharacter::MoveRight(float Value)
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

float AFusionpunksCharacter::GetPlayerHealthPercentage()
{
	return currentHealth / maxHealth;
}

float AFusionpunksCharacter::GetCurrentHealth()
{
	return currentHealth;
}

float AFusionpunksCharacter::GetMaxHealth()
{
	return maxHealth;
}

void AFusionpunksCharacter::ShowCampProgress(ACreepCamp* CurrentCamp)
{
	APlayerHUD* playerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	playerHUD->ShowCampProgress(CurrentCamp);
}

void AFusionpunksCharacter::HideCampProgress()
{
	APlayerHUD* playerHUD = Cast<APlayerHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
	playerHUD->HideCampProgress();
}

void AFusionpunksCharacter::Leap()
{
	LaunchCharacter(FVector::UpVector * leapUpwardForce, true, true);
	timeInAirTimer = leapMaxAirTime;
	bIsLeaping = true;
}




void AFusionpunksCharacter::Dash()
{
	//can dash if leap is not being used
	if (!bIsLeaping)
	{
		dashTimer = dashMaxTime;
		LaunchCharacter(GetActorForwardVector() * dashForce, true, true);
		bIsDashing = true;
	}
}

void AFusionpunksCharacter::ForcePull()
{

}


void AFusionpunksCharacter::StartAttack()
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
		closestEnemy = Results[0].GetActor();
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

void AFusionpunksCharacter::Attack(AActor* enemy)
{

	FDamageEvent DamageEvent;

	float damage = enemy->TakeDamage(20.0f, DamageEvent, Cast<APlayerController>(GetController()), this);


}

void AFusionpunksCharacter::AdjustCameraZoom(float Value)
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