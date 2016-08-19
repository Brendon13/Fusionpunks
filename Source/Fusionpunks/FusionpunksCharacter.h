// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "CreepCamp.h"
#include "FusionpunksCharacter.generated.h"

UCLASS(config=Game)
class AFusionpunksCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AFusionpunksCharacter();

	virtual void BeginPlay() override; 

	virtual void Tick(float DeltaTime) override; 

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

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		float GetPlayerHealthPercentage();
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		float GetCurrentHealth();
	UFUNCTION(BlueprintCallable, Category = HeroFunctions)
		float GetMaxHealth();

protected:
	UPROPERTY(EditAnywhere, Category = Stats) //Make private for final game
	float maxHealth;
private:
	float currentHealth;

protected:
	ACreepCamp* CurrentCreepCamp;
	
public:
	UFUNCTION(BlueprintCallable, Category = UIFunctions)
		void ShowCampProgress(ACreepCamp* CurrentCamp);

	UFUNCTION(BlueprintCallable, Category = UIFunctions)
		void HideCampProgress();


//Variables and functions for Leap Ability 
protected:
	UFUNCTION(BlueprintCallable, Category = Abilities)
		void Leap();

	UPROPERTY(EditAnywhere, Category = LeapAbility)
		float leapUpwardForce;

	UPROPERTY(EditAnywhere, Category = LeapAbility)
		float leapDownwardForce;

	UPROPERTY(EditAnywhere, Category = LeapAbility)
		float leapMaxAirTime;
private:
	float timeInAirTimer;
	bool bIsLeaping;

//Pull Force Ability stuff
protected:
	UFUNCTION(BlueprintCallable, Category = Attack)
		void ForcePull();

	UPROPERTY(EditAnywhere, Category = PullAttack)
		float pullRadius;

	UPROPERTY(EditAnywhere, Category = PullAttack)
		float pullForce;

//Dash Ability Stuff
protected:
	UFUNCTION(BlueprintCallable, Category = Abilities)
		void Dash();

	UPROPERTY(EditAnywhere, Category = DashAbility)
		float dashForce;

	UPROPERTY(EditAnywhere, Category = DashAbility)
		float dashMaxTime;
private:
	float dashTimer;
	bool bIsDashing;

//Attacking
	void StartAttack();
	void Attack(AActor* enemy);

	void AdjustCameraZoom(float Value);

};

