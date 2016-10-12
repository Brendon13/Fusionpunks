// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "FloatingDamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UFloatingDamageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetIncDamage(float Damage) { incDamage = Damage; }

	/*UFUNCTION(BlueprintCallable, Category = Animations)
		void PlayTextFloatUpAnimation();*/
	UFUNCTION(BlueprintCallable, Category = WidgtFunctions)
		void SetOwningPawn(APawn* Owner) { OwningPawn = Owner; }

	UFUNCTION(BlueprintCallable, Category = WidgtFunctions)
		APawn* GetOwningPawn() const { return OwningPawn; }

private:
	UFUNCTION(BlueprintCallable, Category = Combat)
		FText GetIncDamage() const;
	
    float incDamage;
	APawn* OwningPawn;
	
};
