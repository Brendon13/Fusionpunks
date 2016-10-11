// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CreepFormation.generated.h"

<<<<<<< HEAD

=======
>>>>>>> refs/remotes/origin/Master-(Do-Not-Touch)
UENUM(BlueprintType)
enum class EFormationType : uint8
{
	FT_Circle    UMETA(DisplayName = "CircleHeroFormation"),
<<<<<<< HEAD
	FT_Line		UMETA(DisplayerName = "LineFormation")
=======
	FT_Line		 UMETA(DisplayerName = "LineFormation")
>>>>>>> refs/remotes/origin/Master-(Do-Not-Touch)
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FUSIONPUNKS_API UCreepFormation : public UActorComponent
{
	GENERATED_BODY()

public:	
<<<<<<< HEAD
	// Sets default values for this component's properties
	UCreepFormation();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, Category = AIStats)
	EFormationType formationType;

	UPROPERTY(EditDefaultsOnly, Category = AIStats)
		float leaderFollowDistance;

	UPROPERTY(EditDefaultsOnly, Category = AIStats)
		float creepSeparationDistance;

private:
	class AHeroBase* owningHero;
=======
	UCreepFormation();
	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, Category = FormationType)
	EFormationType formationType;

private:
	TArray<FVector> slotArray;
	UPROPERTY(EditDefaultsOnly, Category = CircleFormation)
	float circleRadius;
	UPROPERTY(EditDefaultsOnly, Category = CircleFormation)
	float circleRadiusMultiplier;

public:
	void CalculateSlotPositions(int ArmySize);
	FORCEINLINE FVector GetPositionInFormation(int Index) const { return (Index <= slotArray.Num() ? slotArray[Index - 1] : FVector::ZeroVector); }
protected:
	TArray<AActor*> actorSlotArray;
>>>>>>> refs/remotes/origin/Master-(Do-Not-Touch)

};
