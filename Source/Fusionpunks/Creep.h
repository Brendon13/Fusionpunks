// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "WidgetComponent.h"
#include "Creep.generated.h"

class ACreepCamp;

UCLASS(abstract)
class FUSIONPUNKS_API ACreep : public ACharacter
{
	GENERATED_BODY()

public:
	ACreep();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Stats)
	FORCEINLINE float GetHealthAsDecimal() const  { return currentHealth / maxHealth; }

	UFUNCTION(BlueprintCallable, Category = Stats)
		void LevelUp();

//editable stats 
protected:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float maxHealth;

	float currentHealth;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		int maxLevel;

	int currentLevel;

	UPROPERTY(EditAnywhere, Category = Stats)
		float patrolMovementSpeed;

	UPROPERTY(EditAnywhere, Category = Stats)
		float runSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float attackPower;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float healthIncreasePerLevel;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float attackPowerIncreasePerLevel;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float meleeAttackRange;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float meleeAttackCooldown;

	float meleeAttackCooldownTimer;
	bool bCanMeleeAttack;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float chaseDistance;

	FName team;

public:
	FName GetTeam() const { return team; }
	float GetAttackPower() const { return attackPower; }

	void ClearEnemyTarget();

//Creep Camp stuff
public:
	void SetCreepCampHome(ACreepCamp* home, bool BelongsToCamp);
	bool bBelongsToCamp; //false means the creep belongs to a player (i.e. is apart of a player army)
protected:
	ACreepCamp* creepCampHome;

//health bar widget stuff
protected:
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* widgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<class UCreepHealthbarWidget> CreepHealthBarWidgetClass;

	FRotator widgetCompRotation;
	AActor* localPlayer;


//Damage stuff
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Widgets)
	TSubclassOf<class UFloatingDamageWidget> FloatingDamageWidgetClass; 

public:
	virtual float MeleeAttack();

protected:
//AISTUFF
	/*UPROPERTY(EditDefaultsOnly)
		class ACreepAIController* AiController; */

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float patrolRadius;

	AActor* EnemyTarget;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float agroRadius;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* agroRadiusSphere; 
public:
	UFUNCTION(BlueprintCallable, Category = CampVariables)
		FORCEINLINE float GetPatrolRadius() const { return patrolRadius; }

	UFUNCTION(BlueprintCallable, Category = CampVariables)
		FORCEINLINE ACreepCamp* GetCreepCampHome() const { return (creepCampHome ? creepCampHome : nullptr);}

	void SetToRun(); 
	void SetToWalk();

//stuff for Character Movement
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
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

protected:
	//function for Trigger Events
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* ThisComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//function for Trigger Exit Events
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void JoinPlayerArmy(class AHeroBase* PlayerToFollow, int SlotAssignment);
	int slotAssignment;
	void SetPlayerToFollow(class AHeroBase* Hero) { playerToFollow = Hero; }
	void SetEnemyTarget(AActor* enemy);
private:
	class AHeroBase* playerToFollow;
	float distanceFromCamp;
};
