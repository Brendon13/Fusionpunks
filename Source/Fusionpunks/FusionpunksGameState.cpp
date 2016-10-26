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
	UE_LOG(LogTemp, Warning, TEXT("Diesel Wins!"));

	//DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	for (int i = 0; i < Players.Num(); i++)
	{
		APlayerController* playerController = Cast<APlayerController>(Players[i]->GetController());
		if (playerController)
		{
			Players[i]->DisableInput(playerController);
		}
	
		//for each AI controlled hero... unpossess it 
		/*if (Players[i]->Tags.Contains("AI"))
		{
			Players[i]->GetController()->UnPossess();
		}*/
	}
}

void AFusionpunksGameState::CyberWins()
{
	UE_LOG(LogTemp, Warning, TEXT("Cyber Wins!"));

	//DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	for (int i = 0; i < Players.Num(); i++)
	{
		APlayerController* playerController = Cast<APlayerController>(Players[i]->GetController());
		if (playerController)
		{
			Players[i]->DisableInput(playerController);
		}

		//for each AI controlled hero... unpossess it 
		/*if (Players[i]->Tags.Contains("AI"))
		{
		Players[i]->GetController()->UnPossess();
		}*/
	}
}