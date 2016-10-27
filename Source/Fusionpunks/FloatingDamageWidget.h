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
		void SetOwningActor(AActor* Owner) { OwningActor = Owner; }

	UFUNCTION(BlueprintCallable, Category = WidgtFunctions)
		AActor* GetOwningActor() const { return OwningActor; }

private:
	UFUNCTION(BlueprintCallable, Category = Combat)
		FText GetIncDamage() const;
	
    float incDamage;
	AActor* OwningActor;
	
};
