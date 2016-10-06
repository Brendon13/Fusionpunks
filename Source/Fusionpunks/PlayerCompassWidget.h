// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "PlayerCompassWidget.generated.h"

class AHeroBase;
/**
 * 
 */
UCLASS()
class FUSIONPUNKS_API UPlayerCompassWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:

	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	void SetOwningHero(AHeroBase* OwningHero)
	{
		owningHero = OwningHero;
	}

	UFUNCTION(BlueprintCallable, Category = Functions)
		FLinearColor SetCompassColor() const;

	UPROPERTY(EditAnywhere, Category = Variables)
	float alphaMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	   UWidgetAnimation* PulseAnimation;

protected:
	AHeroBase* owningHero;
	float imageAlpha;
	bool bCountUp;
	
};
