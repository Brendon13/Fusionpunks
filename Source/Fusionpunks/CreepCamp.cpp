// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FusionpunksCharacter.h"
#include "CreepCamp.h"


// Sets default values
ACreepCamp::ACreepCamp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create and Set the Static Mesh Component
	campMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CampMesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> campStaticMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/mk6_goerge.mk6_goerge'"));
	campMesh->SetStaticMesh(campStaticMesh.Object);
	campMesh->bGenerateOverlapEvents = false;
	campMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	RootComponent = campMesh;

	//Create our ring around the camp
	ringMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CaptureRing"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ringStaticMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/CreepCampCircle.CreepCampCircle'"));
	ringMesh->SetStaticMesh(ringStaticMesh.Object);
	ringMesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 3.25f));
	ringMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 5.5f));
	ringMesh->AttachTo(RootComponent);

	//create & Set box trigger for capturing the camp
	sphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerCollider"));
	sphereTrigger->SetSphereRadius(280.0f, true);
	sphereTrigger->bGenerateOverlapEvents = true;
	sphereTrigger->AttachTo(RootComponent);

	//starting status
	status = "neutral";

	//speed at which camp rotates
	ringRotationSpeed = 25.0f;
	ringMaterialAlpha = 0.5f;
	ringMaterialAlphaSpeed = 1.0f;
	bCountUp = true;

	//setting capture camp variables
	captureTime = 5.0f;		//***If capture progress gets to 0 then Cyber captures camp, if captureprogress gets to captureThreshold Diesel captures camp
	dieselCaptureProgress = 0.0f;
	cyberCaptureProgress = 0.0f;
	bCyberIsCapturing = false;
	bDieselIsCapturing = false; 
}

// Called when the game starts or when spawned
void ACreepCamp::BeginPlay()
{
	Super::BeginPlay();

	ringRotation = ringMesh->GetComponentRotation();
	sphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACreepCamp::OnOverlapBegin);
	sphereTrigger->OnComponentEndOverlap.AddDynamic(this, &ACreepCamp::OnOverlapEnd);
	
	//set locations
	creep1SpawnLocation = FVector(this->GetActorLocation().X + 500, this->GetActorLocation().Y, this->GetActorLocation().Z + 50);
	creep2SpawnLocation = FVector(this->GetActorLocation().X - 500, this->GetActorLocation().Y, this->GetActorLocation().Z + 50);
	creep3SpawnLocation = FVector(this->GetActorLocation().X, this->GetActorLocation().Y + 500, this->GetActorLocation().Z + 50);
	creepSpawnArray.Add(creep1SpawnLocation);
	creepSpawnArray.Add(creep2SpawnLocation);
	creepSpawnArray.Add(creep3SpawnLocation);
}

// Called every frame
void ACreepCamp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Rotate the ring every frame
	ringRotation.Yaw += DeltaTime * ringRotationSpeed;
	ringMesh->SetRelativeRotation(ringRotation);

	if (status != "diesel" && bDieselIsCapturing && !bCyberIsCapturing)
	{
		if (ringMaterialAlpha >= 1)
		{
			bCountUp = false;
		}
		else if (ringMaterialAlpha <= 0.1f)
		{
			bCountUp = true;
		}

		if (bCountUp)
		{
			ringMaterialAlpha += DeltaTime * ringMaterialAlphaSpeed;
		}
		else if (!bCountUp)
		{
			ringMaterialAlpha -= DeltaTime * ringMaterialAlphaSpeed;
		}
		//set ring material to fade in and out
		ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), ringMaterialAlpha);

		//check capture progress
		if (cyberCaptureProgress >= 0)
		{
			cyberCaptureProgress -= DeltaTime;
		}
		
		if (cyberCaptureProgress <= 0)
		{
			dieselCaptureProgress += DeltaTime;
		}

		if (dieselCaptureProgress >= captureTime)
		{
			SetToDieselCamp();
		}
	}
	else if (status != "cyber" && !bDieselIsCapturing && bCyberIsCapturing)
	{
		if (ringMaterialAlpha >= 1)
		{
			bCountUp = false;
		}
		else if (ringMaterialAlpha <= 0.1f)
		{
			bCountUp = true;
		}

		if (bCountUp)
		{
			ringMaterialAlpha += DeltaTime * ringMaterialAlphaSpeed;
		}
		else if (!bCountUp)
		{
			ringMaterialAlpha -= DeltaTime * ringMaterialAlphaSpeed;
		}

		//set ring material to fade in and out
		ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), FMath::Sin(ringMaterialAlpha));

		if (dieselCaptureProgress >= 0)
		{
			dieselCaptureProgress -= DeltaTime;
		}

		if (dieselCaptureProgress <= 0)
		{
			cyberCaptureProgress += DeltaTime;
		}

		if (cyberCaptureProgress >= captureTime)
		{
			SetToCyberCamp();
		}
	}
}

//On Trigger Function 
void ACreepCamp::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Entered Camp Trigger"));

	if (OtherActor->Tags.Contains("CyberPlayer") || OtherActor->Tags.Contains("DieselPlayer"))
	{
		if (Cast<AFusionpunksCharacter>(OtherActor))
		{
			AFusionpunksCharacter* heroChar = Cast<AFusionpunksCharacter>(OtherActor);
			heroChar->ShowCampProgress(this);
		}

		if (OtherActor->Tags.Contains("CyberPlayer"))
		{
			bCyberIsCapturing = true; 
		}

		if (OtherActor->Tags.Contains("DieselPlayer"))
		{
			bDieselIsCapturing = true;
		}
	}
	//teamInCampStringArray.Add(OtherActor->Tags[0].ToString());
	/*if (OtherActor->Tags.Contains("DieselPlayer") || OtherActor->Tags.Contains("CyberPlayer"))
	{
		teamInCampStringArray.Add(OtherActor->Tags[0].ToString());

		UE_LOG(LogTemp, Warning, TEXT("Player Entered Camp Trigger"));
		bIsBeingCaptured = true;
	}*/
}


void ACreepCamp::OnOverlapEnd(class AActor * OtherActor, class UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Exited Camp Trigger"));

	if (OtherActor->Tags.Contains("CyberPlayer") || OtherActor->Tags.Contains("DieselPlayer"))
	{
		if (Cast<AFusionpunksCharacter>(OtherActor))
		{
			AFusionpunksCharacter* heroChar = Cast<AFusionpunksCharacter>(OtherActor);
			heroChar->HideCampProgress();
		}

		if (OtherActor->Tags.Contains("CyberPlayer"))
		{
			bCyberIsCapturing = false;
		}

		if (OtherActor->Tags.Contains("DieselPlayer"))
		{
			bDieselIsCapturing = false; 
		}
		ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);
	}
	//if (OtherActor->Tags.Contains("DieselPlayer"))
	//{
	//	//int i = teamInCampStringArray.Find("Diesel");
	//	//teamInCampStringArray[i] == " ";
	//	UE_LOG(LogTemp, Warning, TEXT("Player Exited Camp Trigger"));
	//	bIsBeingCaptured = false;
	//}

	//end capture timer if camp hasn't finished being captured 
}

float ACreepCamp::GetCyberCapturePercentage()
{
	return cyberCaptureProgress / captureTime;
}

float ACreepCamp::GetDieselCapturePercentage()
{
	return dieselCaptureProgress / captureTime;
}

void ACreepCamp::SetToDieselCamp()
{
	//set color and transparency of ring
	ringMesh->SetVectorParameterValueOnMaterials(TEXT("RingColor"), FVector::FVector(0, 0, 0));
	ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);

	status = "diesel";
}

//change camp functionality to cyber function
void ACreepCamp::SetToCyberCamp()
{
	//set color and transparency of ring 
	ringMesh->SetVectorParameterValueOnMaterials(TEXT("RingColor"), FVector::FVector(0.0, 0.0f, 1.0f));
	ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);

	status = "cyber";
}

void ACreepCamp::SetToNeutralCamp()
{
	//set color and transparency of ring
	ringMesh->SetVectorParameterValueOnMaterials(TEXT("RingColor"), FVector::FVector(1.0f, 1.0f, 1.0f));
	ringMesh->SetScalarParameterValueOnMaterials(TEXT("Transparency"), 0.5f);

	status = "neutral";
}