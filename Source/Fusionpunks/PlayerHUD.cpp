// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "PlayerHUD.h"

APlayerHUD::APlayerHUD()
{
	static ConstructorHelpers::FObjectFinder<UClass>
		healthWidgetFinder(TEXT("/Game/UI/PlayerHealthWidget_BP.PlayerHealthWidget_BP_C"));

	if (healthWidgetFinder.Object != NULL)
	{
		playerHealthBarWidgetClass = Cast<UClass>(healthWidgetFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass>
		campPWFinder (TEXT("/Game/UI/CampProgressWidget_BP.CampProgressWidget_BP_C"));

	if (campPWFinder.Object != NULL)
	{
		campProgressWidgetClass = Cast<UClass>(campPWFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UClass> gameTWFinder(TEXT("/Game/UI/GameTimerWidget_BP.GameTimerWidget_BP_C"));

	if (gameTWFinder.Object != NULL)
	{
		gameTimerWidgetClass = Cast<UClass>(gameTWFinder.Object);
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

