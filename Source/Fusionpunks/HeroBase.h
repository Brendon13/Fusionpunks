// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "HeroBase.generated.h"

UCLASS(abstract)
class FUSIONPUNKS_API AHeroBase : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
		AActor* AICam;

public:
	AHeroBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
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
	void Attack(AActor* enemy);

protected:
	void StartAttack();
	
	void AdjustCameraZoom(float Value);
	void SwapAICamera();

	//editable stats in blueprint
protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float maxHealth;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int maxLevel;

	UPROPERTY(EditAnywhere, Category = Stats)
		float movementSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float basicAttackDamage;
	

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float healthIncreasePerLevel;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float damageIncreasePerLevel;

	UPROPERTY(VisibleAnywhere)
		float currentHealth;
	UPROPERTY(VisibleAnywhere)
		float healthPercent;

	int32 currentLevel;
	


	UFUNCTION(BlueprintCallable, category = Stats)
		virtual void LevelUp();

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float agroRadius;
	

public:
	UFUNCTION(BlueprintCallable, Category = UIFunctions)
		void ShowCampProgress(ACreepCamp* CurrentCamp);

	UFUNCTION(BlueprintCallable, Category = UIFunctions)
		void HideCampProgress();

	void ResetHealth();

	FVector startingLocation;

	bool bIsRespawning = false;

	FORCEINLINE int32 GetArmySize() const { return currentArmySize; }
	FORCEINLINE float GetRespawnTime() const { return respawnTime; }
	FORCEINLINE  int32 GetLevel() const { return currentLevel; }
	FORCEINLINE  float GetAttackDamage() const { return basicAttackDamage; }
	FORCEINLINE TArray<class ACreepCamp*> GetCapturedCamps() const { return capturedCamps; }
	FORCEINLINE class HeroStats* GetHeroStats() const { return heroStats; }
	void AddToCapturedCamps(class ACreepCamp* camp);
	void RemoveFromCapturedCamps(class ACreepCamp* camp);
	void UpdateHeroStats();
protected:
	class ACreepCamp* CurrentCreepCamp;

	UPROPERTY(EditDefaultsOnly, Category = Respawn)
		TSubclassOf<class ARespawnOverTime> respawnClass;

	class ARespawnOverTime* respawnEffect;

	UPROPERTY(EditDefaultsOnly, Category = Respawn)
		float respawnTime;
	
	class HeroStats* heroStats;

	TArray<class ACreepCamp*> capturedCamps;
	
private:
	//function for Trigger Events
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//function for Trigger Exit Events
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = CollisionComponents)
		USphereComponent* sphereTrigger;

	void LinkToCreepCamps();

protected:
	 FName team;
	 UPROPERTY(EditDefaultsOnly)
		 TSubclassOf<ACreepCamp> creepCampClass;
public:
	FORCEINLINE FName GetTeam() const { return team; }

	UPROPERTY(EditDefaultsOnly)
		class UWidgetComponent* widgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
		TSubclassOf<class UPlayerCompassWidget> CompassWidgetClass;

//AI Stuff
protected:
	virtual void RecruitCreep();
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int32 maxArmySize;

public:
	
	//AI HERO FUNCTIONS
	// USE ONLY WITH AI HERO TASKS!!

	bool CheckForNearbyEnemyCreeps();

	bool CheckForNearbyEnemyHero();

	FORCEINLINE TArray<class ACreep*> GetNearbyEnemyCreeps() const { return nearbyEnemyCreeps; }
	

private:
	int32 currentArmySize;
	
	ACreepCamp* visitingCamp;
	TArray<class ACreep*> CreepArmy;


	//AIHERO
	TArray<class ACreep*> nearbyEnemyCreeps;
	AHeroBase* nearbyEnemyHero;


public:
	TArray<ACreep*> AHeroBase::GetCreepArmyArray();
	UPROPERTY(EditDefaultsOnly)
	class UCreepFormation* creepFormationComp;

};
