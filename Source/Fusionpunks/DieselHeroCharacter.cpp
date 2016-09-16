// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Fusionpunks.h"
#include "PlayerHUD.h"
#include "DieselHeroCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AFusionpunksCharacter

ADieselHeroCharacter::ADieselHeroCharacter()
{
	
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
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ADieselHeroCharacter::OnHit);
}

void ADieselHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ADieselHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLeaping)
	{
		timeInAirTimer -= DeltaTime;

		if (timeInAirTimer <= 0)
		{
			LaunchCharacter(FVector::UpVector * -1 * leapDownwardForce, true, true);
		}
	}

	if (bIsDashing)
	{
		dashTimer -= DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("Is Dashing!"));
		if (dashTimer <= 0)
		{
			bIsDashing = false;
			//UE_LOG(LogTemp, Warning, TEXT("Stop Dashing Force!"));
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
void ADieselHeroCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	Super::SetupPlayerInputComponent(InputComponent);
	// Set up gameplay key bindings
	check(InputComponent);
	

	InputComponent->BindAction(TEXT("Leap"), IE_Pressed, this, &ADieselHeroCharacter::Leap);
	InputComponent->BindAction(TEXT("ForcePull"), IE_Pressed, this, &ADieselHeroCharacter::ForcePull);
	InputComponent->BindAction("Dash", IE_Pressed, this, &ADieselHeroCharacter::Dash);

	

}







void ADieselHeroCharacter::Leap()
{
	LaunchCharacter(FVector::UpVector * leapUpwardForce, true, true);
	timeInAirTimer = leapMaxAirTime;
	bIsLeaping = true;
}

void ADieselHeroCharacter::Dash()
{
	//can dash if leap is not being used
	if (!bIsLeaping)
	{
		dashTimer = dashMaxTime;
		LaunchCharacter(GetActorForwardVector() * dashForce, true, true);
		bIsDashing = true;
	}
}

void ADieselHeroCharacter::ForcePull()
{

}


void ADieselHeroCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->Tags.Contains("Ground"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("HIT GROUND AFTER LEAP! SPAWN DUST!"));
		if (bIsLeaping)
		{
			bIsLeaping = false;

			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Instigator = Instigator;
			SpawnParameters.Owner = this;

			ADustParticleEffect* particleEffect = (ADustParticleEffect*)GetWorld()->SpawnActor<ADustParticleEffect>
				(dustParticleSystemClass,
					GetActorLocation(),
					FRotator::ZeroRotator,
					SpawnParameters);
		}
	}
}