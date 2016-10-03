// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "PlayerHUD.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint>
		healthWidgetFinder(TEXT("WidgetBlueprint'/Game/UI/PlayerHealthWidget_BP.PlayerHealthWidget_BP'"));

	if (healthWidgetFinder.Object != nullptr)
	{
		playerHealthBarWidgetClass = Cast<UClass>(healthWidgetFinder.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint>
		campPWFinder (TEXT("WidgetBlueprint'/Game/UI/CampProgressWidget_BP.CampProgressWidget_BP'"));

	if (campPWFinder.Object != nullptr)
	{
		campProgressWidgetClass = Cast<UClass>(campPWFinder.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> gameTWFinder(TEXT("WidgetBlueprint'/Game/UI/GameTimerWidget_BP.GameTimerWidget_BP'"));

	if (gameTWFinder.Object != nullptr)
	{
		gameTimerWidgetClass = Cast<UClass>(gameTWFinder.Object->GeneratedClass);
	}

}

void APlayerHUD::BeginPlay()
{
	if (GetWorld()->GetFirstPlayerController()->GetCharacter() != nullptr)
	{
		playerHealthBarWidget = CreateWidget<UPlayerHealthBarWidget>(GetOwningPlayerController(), playerHealthBarWidgetClass);
		playerHealthBarWidget->AddToViewport();

		campProgressWidget = CreateWidget<UCampProgressWidget>(GetOwningPlayerController(), campProgressWidgetClass);
		campProgressWidget->AddToViewport();
		campProgressWidget->SetVisibility(ESlateVisibility::Hidden);

		gameTimerWidget = CreateWidget<UGameTimerWidget>(GetOwningPlayerController(), gameTimerWidgetClass);
		gameTimerWidget->AddToViewport();
	}
	/*if (GetWorld()->GetName() == "MechanicsTesting")
	{
		
	}*/
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
}

void APlayerHUD::ShowCampProgress(ACreepCamp* CurrentCamp)
{
	campProgressWidget->SetCurrentCreepCamp(CurrentCamp);
	campProgressWidget->SetVisibility(ESlateVisibility::Visible);
}

void APlayerHUD::HideCampProgress()
{
	campProgressWidget->SetCurrentCreepCamp(nullptr);
	campProgressWidget->SetVisibility(ESlateVisibility::Hidden);
}

