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

private:
	UFUNCTION(BlueprintCallable, Category = Combat)
		FText GetIncDamage() const;
	
    float incDamage;
//	float timer;
//	bool bIsAnimating;
//
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
//		class UWidgetAnimation* textFloatUpAnimation;
	

	
};
