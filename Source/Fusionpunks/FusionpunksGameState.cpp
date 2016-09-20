// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
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

int AFusionpunksGameState::GetCyberLevel() const
{
	return CyberLevel;
}

int AFusionpunksGameState::GetDieselLevel() const
{
	return DieselLevel;
}