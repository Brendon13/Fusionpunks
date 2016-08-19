// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/Character.h"
#include "Tower.h"
#include "ChainLightning.h"
#include "MobaCharacter.generated.h"

UCLASS(config=Game)
class AMobaCharacter: public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMobaCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);
	float currHP, maxHp, percentHP;


	
	UPROPERTY(EditAnywhere, category = Ability)
		TSubclassOf<AAbilityBase> chainLightningAbility;
	
	TArray<AActor*> affectedActors;
	bool skillSelected = false;



public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	float GetHpPercent();
	float GetCurrHP();
	float GetMaxHP();
	void ApplyDamage(float dmg);
	void DetermineClickEvent();
	void StartAttack();
	void OnSkillPressed();
	void UseSkill(AActor* enemy);
	void Attack(AActor* enemy);
	void AddAffectedActor(AActor* enemy);
	bool IsAffected(AActor* enemy);
	void HighlightTarget(AActor* enemy, TArray<FOverlapResult> enemies);
	void UnHighlightTarget(AActor* enemy);
	void UnHighlightAll(TArray<FOverlapResult> enemies);
	AActor* UpdateTarget();
	void AdjustCameraZoom(float Value);

//Skill Target related
private:
	TArray<FOverlapResult> skillTargetResults;
	TArray<FOverlapResult> oldTargetResults;

	
		
	
};

