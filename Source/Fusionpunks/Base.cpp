// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FloatingDamageWidget.h"
#include "FusionpunksGameState.h"
#include "Base.h"



// Sets default values
ABase::ABase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//rootCldr = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseCollider"));
	//rootCldr->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	baseMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	healthBar = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBar"));
	healthBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	/*const ConstructorHelpers::FObjectFinder<UBlueprint>
		FloatingDamageWidgetFinder(TEXT("WidgetBlueprint'/Game/UI/FloatingDamageWidget_BP.FloatingDamageWidget_BP'"));

	if (FloatingDamageWidgetFinder.Object != nullptr)
	{
		FloatingDamageWidgetClass = Cast<UClass>(FloatingDamageWidgetFinder.Object->GeneratedClass);
	}*/

	bCanBeDamaged = true;
	currHP = maxHP;

}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();
	
	//playerCam = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void ABase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), playerChar->GetActorLocation());
	//healthBar->SetWorldRotation(newRotation);	
}


float ABase::TakeDamage(float DamageAmount,struct FDamageEvent const & DamageEvent,class AController * EventInstigator,AActor * DamageCauser)
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
	
	UE_LOG(LogTemp, Log, TEXT("Base took %f damage."), DamageAmount);
    
	if (currHP <= 0)
	{
		//Destroy();

		if (Tags.Contains("CyberBase"))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Diesel Wins!"));
			AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
			if (gameState)
			{
				gameState->DieselWins();
			}

		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Cyber Wins!"));
			AFusionpunksGameState* gameState = Cast<AFusionpunksGameState>(GetWorld()->GetGameState());
			if (gameState)
			{
				gameState->CyberWins();
			}
		}
	}
	return DamageAmount;
}

float ABase::GetHpPercent() 
{
	
	return currHP / maxHP;
}

