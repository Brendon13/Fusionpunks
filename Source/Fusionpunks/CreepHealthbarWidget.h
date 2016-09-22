// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CreepHealthbarWidget.generated.h"

class ACreepCamp;

UCLASS()
class FUSIONPUNKS_API UCreepHealthbarWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetOwningCreep(APawn* OwningPawn) 
	{
		owningPawn = OwningPawn;
	}

protected:
	UFUNCTION(BlueprintCallable, Category = Creep)
		float GetCreepHealthAsDecimal() const;

	APawn* owningPawn;

};
