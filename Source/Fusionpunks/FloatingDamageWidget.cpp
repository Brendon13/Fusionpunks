// Fill out your copyright notice in the Description page of Project Settings.

#include "Fusionpunks.h"
#include "FloatingDamageWidget.h"


//void UFloatingDamageWidget::NativeConstruct()
//{
//	bCanEverTick = true; 
//	timer = 2.0f;
//	bIsAnimating = false; 
//}
//
//void UFloatingDamageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//
//	if (bIsAnimating)
//	{
//		timer -= InDeltaTime;
//
//		if (timer < 0)
//		{
//			RemoveFromParent();
//		}
//	}
//}
//
//void UFloatingDamageWidget::PlayTextFloatUpAnimation()
//{
//	PlayAnimation(textFloatUpAnimation);
//	bIsAnimating = true;
//}

FText UFloatingDamageWidget::GetIncDamage() const
{
	return FText::AsNumber(incDamage);
}

