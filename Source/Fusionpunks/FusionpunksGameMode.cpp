// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Fusionpunks.h"
#include "FusionpunksGameMode.h"
#include "FusionpunksGameState.h"
#include "PlayerHUD.h"
#include "DieselHeroCharacter.h"

AFusionpunksGameMode::AFusionpunksGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Blueprints/Heroes/DieselCharacter.DieselCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/

	HUDClass = APlayerHUD::StaticClass();
	GameStateClass = AFusionpunksGameState::StaticClass();
}

