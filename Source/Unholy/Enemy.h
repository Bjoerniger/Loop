// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "DestructibleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_range 	UMETA(DisplayName = "shoting attack"),
	EAT_allert 	UMETA(DisplayName = "allerting allies"),
	EAT_charge	UMETA(DisplayName = "charge attack"),
	EAT_defense	UMETA(DisplayName = "defend allies"),
	EAT_mines	UMETA(DisplayName = "mines"),
	EAT_walker	UMETA(DisplayName = "walker"),
	EAT_clean	UMETA(DisplayName = "clean")
};

UENUM(BlueprintType)
enum class ELootTable : uint8
{
	ELT_boss 	UMETA(DisplayName = "boss loot"),
	ELT_midBoss UMETA(DisplayName = "mid-boss loot"),
	ELT_trash	UMETA(DisplayName = "trash mob loot"),
	ELT_clean	UMETA(DisplayName = "clean")
};


UENUM(BlueprintType)
enum class EAggroState : uint8
{
	EAS_neutral UMETA(DisplayName = "neutral state"),
	EAS_allert 	UMETA(DisplayName = "allerted aggro state"),
	EAS_attack	UMETA(DisplayName = "attacking aggro state"),
	EAS_defense	UMETA(DisplayName = "defending aggro state"),
	EAS_patrol  UMETA(DisplayName = "patrol state"),
	EAS_clean	UMETA(DisplayName = "clean")
};

UENUM(BlueprintType)
enum class ENonAggroState : uint8
{
	ENA_idle 	UMETA(DisplayName = "idle state"),
	ENA_patrol 	UMETA(DisplayName = "patroling state"),
	ENA_combat 	UMETA(DisplayName = "combat state"),
	ENA_clean	UMETA(DisplayName = "clean")
};

UCLASS()
class UNHOLY_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	

private:
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	float DistanceTo(FVector location);

	UFUNCTION(BlueprintCallable)
	void AllerterAttack(FVector playerPosition);

	UFUNCTION(BlueprintCallable)
	void MovementToLocation(FVector destination);

	UFUNCTION(BlueprintCallable)
	int CheckPathToTarget();

	UFUNCTION(BlueprintCallable)
	void ShooterAttack(AActor* target);

	UFUNCTION(BlueprintCallable)
	void WalkerAttack(AActor* target);

	UFUNCTION(BlueprintCallable)
	void ChargerAttack();

	UFUNCTION(BlueprintCallable)
	void AimChasis();

	UFUNCTION(BlueprintCallable)
	void AimWalkerChasis();

	UFUNCTION(BlueprintCallable)
	void AimWeapon();

	UFUNCTION(BlueprintCallable)
	void Aggro(AActor* target);

	UFUNCTION(BlueprintCallable)
	void FleeYouFewl();

	UFUNCTION(BlueprintCallable)
	void Defend();

	UFUNCTION(BlueprintCallable)
	int GetDirectionToPlayer();

	UFUNCTION()
	int DirectionToLocation(FVector location);

	UFUNCTION(BlueprintCallable)
	bool LOSCheck(AActor* actor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector originBeforeEvade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector datLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bShowLOSPos;

	// overlap component for dmg
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* bodyTriggerVolume;

	// overlap component for dmg
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* headTriggerVolume;

	// overlap component for aggro
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* aggroSphere;


	// independant from class, goes for ALL classes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	bool bIsPatroling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	TArray<ATargetPoint*> patrolWayPoints;

	// filled on start via BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	TArray<FVector> patrolWayPointLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	int currentWayPoint;

	// true fw, false bw
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	bool patrolDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	bool bDoesPatrol;

	// true requires first and last WP to be the same
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	bool bPatrolReturn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	FVector aggroOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolResumeTimer;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FRotator theAimRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bGetInLOS;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector LOSVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector intersectionLOS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector hitResultLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<AActor*> alliedEnemiesInRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bGoTo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsFleeing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsIntercepting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsEvading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool newDir;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector reachLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int fixedMovementAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bActivateShield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float shieldActiveTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float shieldActiveTimerValue;

	/*The Component which is used for the "seeing" sense of the AI*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UPawnSensingComponent* pawnSensingComp;

	/*The Behavior Tree of the Character*/
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAttackType attackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EAggroState	aggroState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	ELootTable lootTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	ENonAggroState nonAggroState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bIsAllerted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float allertRange;

	// 0: normal, 1: y only, 2: z only;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int movementPattern;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector movementDestination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float inputValueX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float inputValueZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float valueY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float valueZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float speedValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float distanceToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float distanceToLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FVector lastKnownPlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AActor* horizonCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UArrowComponent* upperMeshBorder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UArrowComponent* lowerMeshBorder;

	// attack range
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float minRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float combatRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	float aggroRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bPlayerIsInAggroRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bCanSeePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bDoFace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int directionToPlayer;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float defenderShieldHealthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float defenderShieldHealthValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float defenderShieldRegenTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float defenderShieldRegenTimerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float defenderShieldRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int fixedMovementAxisMines;

	// shooter vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int fixedMovementAxisShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float speedValueShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float minRangeShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float combatRangeShooter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroInitTimerMaxShooter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroOverTimerMaxShooter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroRangeShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int evadeCounterShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeTimerMaxShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeDistanceShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeSpeedShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroHeightShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float healthMaxShooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanEvadeShooter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolSpeedMultiplierShooter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	bool bCanReceiveCriticalHitShooter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float shotFrequencyShooter;

	// allerter vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float speedValueAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int fixedMovementAxisAllerter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroRangeAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float combatRangeAllerter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroInitTimerMaxAllerter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroOverTimerMaxAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float minRangeAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float healthMaxAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanEvadeAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int evadeCounterAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeTimerMaxAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeDistanceAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeSpeedAllerter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroHeightAllerter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	bool bCanReceiveCriticalHitAllerter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolSpeedMultiplierAllerter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float shotFrequencyAllerter;

	// charger vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int fixedMovementAxisCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float minRangeCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float combatRangeCharger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroInitTimerMaxCharger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroOverTimerMaxCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float speedValueCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float healthMaxCharger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroRangeCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanEvadeCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int evadeCounterCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeTimerMaxCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeDistanceCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeSpeedCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroHeightCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolSpeedMultiplierCharger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	bool bCanReceiveCriticalHitCharger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float shotFrequencyCharger;

	// defender vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int fixedMovementAxisDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float speedValueDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float minRangeDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float combatRangeDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroInitTimerMaxDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroOverTimerMaxDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float healthMaxDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroRangeDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanReceiveCrititcalHitDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanEvadeDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int evadeCounterDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeTimerMaxDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeDistanceDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeSpeedDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroHeightDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolSpeedMultiplierDefender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float shotFrequencyDefender;

	// mines vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float speedValueMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float combatRangeMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float minRangeMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroInitTimerMaxMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroOverTimerMines;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroOverTimerMaxMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float healthMaxMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanReceiveCriticalHitMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanEvadeMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int evadeCounterMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroHeightMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeTimerMaxMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeSpeedMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeDistanceMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroRangeMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolSpeedMultiplierMines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float shotFrequencyMines;

	// walker vars

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int fixedMovementAxisWalker;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float minRangeWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float combatRangeWalker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroInitTimerMaxWalker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroOverTimerMaxWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float speedValueWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float healthMaxWalker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	float aggroRangeWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	bool bCanEvadeWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	int evadeCounterWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeTimerMaxWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeDistanceWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float evadeSpeedWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float aggroHeightWalker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "patrol")
	float patrolSpeedMultiplierWalker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	bool bCanReceiveCriticalHitWalker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	bool bAimWalkerChasis;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	FRotator walkerChasisRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "class")
	bool bWalkerIsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "class")
	float shotFrequencyWalker;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bIsInCombatRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	float aggroHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vars")
	float aggroOverTimerValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vars")
	float aggroOverTimerMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vars")
	bool bCanReceiveCriticalHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vars")
	float aggroInitTimerValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "vars")
	float aggroInitTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	float healthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	float healthValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bCanShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	float shotFrequencyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	float shotFrequencyMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	ACharacter* thePlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bIsKickable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bBeenKicked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bDoExplode;

	// anims
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bIsWalking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bDoJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	bool bIsHomingTarget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "evasion")
	bool bCanEvade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "evasion")
	int evadeCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "evasion")
	float evadeTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "evasion")
	float evadeTimerActual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "evasion")
	float evadeDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "evasion")
	float evadeSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damageCast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float healthReturnValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dashedTimerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dashedTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBeenDashed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGotGrav;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* theHitMan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimitiveComponent* theHitComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBeenHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBeenCrit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDisplayCritEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector playerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* enemyModel_static;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* enemyModel_weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDestructibleComponent* model_destruct;

};
