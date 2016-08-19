// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "Base.h"
#include "Tower.h"
#include "BaseHealthBar.h"



float UBaseHealthBar::GetHpPercent() const {
	if (owningActor != nullptr) 
	{
		if (owningActor->IsA(ABase::StaticClass()))
			return (Cast<ABase>(owningActor))->GetHpPercent();
		else if (owningActor->IsA(ATower::StaticClass()))
			return (Cast<ATower>(owningActor))->GetHpPercent();
		else
			UE_LOG(LogTemp, Display, TEXT("CantFindActorClass"));
			return 1;
	}
	else	
		UE_LOG(LogTemp, Display, TEXT("OwningActorIsNull"));
		return 1;
	
}

AActor* UBaseHealthBar::GetOwningActor()
{
	return owningActor;
}

void UBaseHealthBar::SetOwningActor(AActor* owner) {
	owningActor = owner;
}




