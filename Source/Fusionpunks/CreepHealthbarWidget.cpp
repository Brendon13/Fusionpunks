// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Creep.h"
#include "CreepHealthbarWidget.h"

float UCreepHealthbarWidget::GetCreepHealthAsDecimal() const
{
	if (owningPawn != nullptr)
	{
		ACreep* myCreep = Cast<ACreep>(owningPawn);
		return myCreep->GetHealthAsDecimal();
	}
	UE_LOG(LogTemp, Warning, TEXT("Creep reference in CreepHealthbarWidget is NULL!"));
	return 0;
}


