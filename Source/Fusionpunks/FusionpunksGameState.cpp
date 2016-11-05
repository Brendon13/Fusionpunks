// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "HeroBase.h"
#include "FusionpunksGameState.h"



AFusionpunksGameState::AFusionpunksGameState()
{
	CyberLevel = 1;
	DieselLevel = 1;
}

void AFusionpunksGameState::CyberLevelUp()
{
	CyberLevel++;
}

void AFusionpunksGameState::DieselLevelUp()
{
	DieselLevel++;
}

void AFusionpunksGameState::DieselWins()
{
	for (int i = 0; i < Players.Num(); i++)
	{
		APlayerController* playerController = Cast<APlayerController>(Players[i]->GetController());
		if (playerController)
		{
			//disable user input
			Players[i]->DisableInput(playerController);

			//show mouse
			playerController->bShowMouseCursor = true;
			playerController->bEnableClickEvents = true; 
			playerController->bEnableMouseOverEvents = true; 

			//remove all previous widgets from viewport
			GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();

			//add team that won widget
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), DieselWinsWidgetClass);
			widget->AddToViewport();
			continue;
		}
	
		//for each AI controlled hero... unpossess it 
		if (Players[i]->Tags.Contains("AI"))
		{
			AController* AIController = Players[i]->GetController();
			if (AIController)
			{
				AIController->UnPossess();
			}
		}
	}
}

void AFusionpunksGameState::CyberWins()
{
	for (int i = 0; i < Players.Num(); i++)
	{
		APlayerController* playerController = Cast<APlayerController>(Players[i]->GetController());
		if (playerController)
		{
			//disable user input
			Players[i]->DisableInput(playerController);

			//show mouse
			playerController->bShowMouseCursor = true;
			playerController->bEnableClickEvents = true;
			playerController->bEnableMouseOverEvents = true;

			//remove all previous widgets from viewport
			GetWorld()->GetGameViewport()->RemoveAllViewportWidgets();

			//add team that won widget
			UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), CyberWinsWidgetClass);
			widget->AddToViewport();
			continue;
		}

		//for each AI controlled hero... unpossess it 
		if (Players[i]->Tags.Contains("AI"))
		{
			AController* AIController = Players[i]->GetController();
			if (AIController)
			{
				AIController->UnPossess();
			}
		}
	}
}