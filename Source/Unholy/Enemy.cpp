// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "Enemy_AIController.h"
#include "UObject/UObjectBaseUtility.h"
#include <Runtime\Engine\Classes\Kismet\KismetMathLibrary.h>
#include "UnholyCharacter.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bodyTriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("BodyTriggerVolume"));
	bodyTriggerVolume->SetupAttachment(RootComponent);
	bodyTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	bodyTriggerVolume->bEditableWhenInherited = true;
	//bodyTriggerVolume->SetRelativeLocation(FVector(0.f, 0.f, -250.f));

	enemyModel_static = CreateDefaultSubobject<UStaticMeshComponent>(FName("model_static"));
	enemyModel_static->SetupAttachment(bodyTriggerVolume);
	enemyModel_static->bEditableWhenInherited = true;

	enemyModel_weapon = CreateDefaultSubobject<UStaticMeshComponent>(FName("model_weapon"));
	enemyModel_weapon->SetupAttachment(enemyModel_static);
	enemyModel_weapon->bEditableWhenInherited = true;
	
	headTriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("HeadTriggerVolume"));
	headTriggerVolume->SetupAttachment(enemyModel_weapon);
	headTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	headTriggerVolume->bEditableWhenInherited = true;
	//headTriggerVolume->SetRelativeLocation(FVector(0.f, 100.f, 0.f));

	model_destruct = CreateDefaultSubobject<UDestructibleComponent>(FName("model_destruct"));
	model_destruct->SetupAttachment(RootComponent);
	model_destruct->bEditableWhenInherited = true;

	GetCharacterMovement()->DefaultLandMovementMode == EMovementMode::MOVE_Flying;
	GetCharacterMovement()->AirControl = 1.f;

	//Initializing the pawn sensing component
	pawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	//Set the peripheral vision angle to 90 degrees
	pawnSensingComp->SetPeripheralVisionAngle(180.f);
	pawnSensingComp->HearingThreshold = 100.f;
	pawnSensingComp->LOSHearingThreshold = 100.f;
	pawnSensingComp->SightRadius = 100.f;

	upperMeshBorder = CreateDefaultSubobject<UArrowComponent>(TEXT("BorderUp"));
	upperMeshBorder->SetupAttachment(enemyModel_static);
	upperMeshBorder->bEditableWhenInherited = true;
	lowerMeshBorder = CreateDefaultSubobject<UArrowComponent>(TEXT("BorderLow"));
	lowerMeshBorder->SetupAttachment(enemyModel_static);
	lowerMeshBorder->bEditableWhenInherited = true;

	// set in BP till you know how to collision
	//aggroSphere = CreateDefaultSubobject<USphereComponent>(FName("aggroSphere"));
	//if (!ensure(aggroSphere != nullptr)) return;
	//aggroSphere->AttachTo(TriggerVolume);
	//aggroSphere->SetSphereRadius(400.0f);
	//aggroSphere->SetGenerateOverlapEvents(true);
	//aggroSphere->SetCollisionProfileName(TEXT("Pawn"));
	//aggroSphere->bEditableWhenInherited = true;
	//aggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);


	// class variables

	healthMaxAllerter = 15.f;
	healthMaxCharger = 5.f;
	healthMaxShooter = 12.f;
	healthMaxDefender = 20.f;
	healthMaxMines = 10.f;
	healthMaxWalker = 15.f;

	aggroInitTimerMaxAllerter = 1.f;
	aggroInitTimerMaxCharger = .3f;
	aggroInitTimerMaxShooter = .6f;
	aggroInitTimerMaxDefender = .2f;
	aggroInitTimerMaxMines = 3.f;
	aggroInitTimerMaxWalker = 1.f;

	aggroOverTimerMaxAllerter = 2.f;
	aggroOverTimerMaxCharger = 1.f;
	aggroOverTimerMaxShooter = 4.f;
	aggroOverTimerMaxDefender = 3.f;
	aggroOverTimerMaxMines = 5.f;
	aggroOverTimerMaxWalker = 4.f;
	
	aggroRangeAllerter = 1000.f;
	aggroRangeCharger = 800.f;
	aggroRangeShooter = 800.f;
	aggroRangeDefender = 1000.f;
	aggroRangeMines = 1100.f;
	aggroRangeWalker = 1000.f;

	aggroHeight = 500.f;
	aggroHeightAllerter = 500.f;
	aggroHeightCharger = 500.f;
	aggroHeightShooter = 500.f;
	aggroHeightDefender = 500.f;
	aggroHeightMines = 500.f;
	aggroHeightWalker = 50.f;

	minRangeAllerter = 500.f;
	minRangeCharger = 120.f;
	minRangeShooter = 500.f;
	minRangeDefender = 400.f;
	minRangeMines = 300.f;
	minRangeWalker = 600.f;

	combatRangeAllerter = 100.f;
	combatRangeCharger = 0.f;
	combatRangeShooter = 100.f;
	combatRangeDefender = 100.f;
	combatRangeMines = 200.f;
	combatRangeWalker = 300.f;

	bCanReceiveCriticalHitAllerter = true;
	bCanReceiveCriticalHitCharger = false;
	bCanReceiveCriticalHitShooter = true;
	bCanReceiveCrititcalHitDefender = true;
	bCanReceiveCriticalHitMines = true;
	bCanReceiveCriticalHitWalker = true;

	fixedMovementAxis = 0;
	fixedMovementAxisAllerter = 1;
	fixedMovementAxisCharger = 0;
	fixedMovementAxisShooter = 0;
	fixedMovementAxisDefender = 0;
	fixedMovementAxisMines = 0;
	fixedMovementAxisWalker = 1;

	bCanEvadeAllerter = true;
	bCanEvadeCharger = false;
	bCanEvadeShooter = true;
	bCanEvadeDefender = true;
	bCanEvadeMines = false;
	bCanEvadeWalker = false;

	evadeCounterAllerter = 2;
	evadeCounterCharger = 0;
	evadeCounterShooter = 1;
	evadeCounterDefender = 1;
	evadeCounterMines = 0;
	evadeCounterWalker = 0;

	evadeTimerMaxAllerter = 3.f;
	evadeTimerMaxCharger = 3.f;
	evadeTimerMaxShooter = 3.f;
	evadeTimerMaxDefender = 3.f;
	evadeTimerMaxMines = 3.f;
	evadeTimerMaxWalker = 3.f;

	evadeDistanceAllerter = 150.f;
	evadeDistanceCharger = 150.f;
	evadeDistanceShooter = 150.f;
	evadeDistanceDefender = 150.f;
	evadeDistanceMines = 150.f;
	evadeDistanceWalker = 150.f;

	evadeSpeedAllerter = 4.f;
	evadeSpeedCharger = 4.f;
	evadeSpeedShooter = 4.f;
	evadeSpeedDefender = 4.f;
	evadeSpeedMines = 4.f;
	evadeSpeedWalker = 1.f;

	defenderShieldRegenTimerMax = 3.f;

	bIsIntercepting = false;

	bPatrolReturn = false;
	patrolSpeedMultiplier = .1f;
	aggroOrigin = FVector(0.f, 0.f, 0.f);
	patrolResumeTimer = 5.f;
	currentWayPoint = 0;

	speedValueAllerter = 600.f;
	speedValueCharger = 1200.f;
	speedValueShooter = 500.;
	speedValueDefender = 500.f;
	speedValueMines = 300.f;
	speedValueWalker = 650.f;

	shotFrequencyAllerter = 10.f;
	shotFrequencyCharger = 10.f;
	shotFrequencyShooter = 1.f;
	shotFrequencyDefender = 10.f;
	shotFrequencyMines = 3.f;
	shotFrequencyWalker = .3f;

	patrolSpeedMultiplierDefender = .5f;
	patrolSpeedMultiplierCharger = 1.f;
	patrolSpeedMultiplierAllerter = .5f;
	patrolSpeedMultiplierShooter = .75f;
	patrolSpeedMultiplierMines = .5f;
	patrolSpeedMultiplierWalker = .75f;

	bIsWalking = false;
	bDoJump = false;
	bAimWalkerChasis = false;

	bIsHomingTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay() {
	Super::BeginPlay();


	if (pawnSensingComp) {
		pawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::OnSeePlayer);
	}

	// class dependant settings
	switch (attackType) {
		case EAttackType::EAT_allert:
			healthMax = healthMaxAllerter;
			aggroInitTimerMax = aggroInitTimerMaxAllerter;							// time needed to react aggressive
			aggroOverTimerMax = aggroOverTimerMaxAllerter;							// time that enemy stays aggro and tries to follow last position
			aggroRange = aggroRangeAllerter;
			aggroHeight = aggroHeightAllerter;
			bIsKickable = true;														// if enemy can be kicked by player
			speedValue = speedValueAllerter;
			minRange = minRangeAllerter;
			bCanReceiveCriticalHit = bCanReceiveCriticalHitAllerter;
			fixedMovementAxis = fixedMovementAxisAllerter;
			shieldActiveTimerMax = .0f;	
			bCanEvade = bCanEvadeAllerter;
			evadeCounter = evadeCounterAllerter;
			evadeTimerMax = evadeTimerMaxAllerter;
			evadeDistance = evadeDistanceAllerter;
			evadeSpeed = evadeSpeedAllerter;
			combatRange = combatRangeAllerter;
			patrolSpeedMultiplier = patrolSpeedMultiplierAllerter;
			shotFrequencyMax = shotFrequencyAllerter;
			break;

		case EAttackType::EAT_charge:
			healthMax = healthMaxCharger;
			aggroInitTimerMax = aggroInitTimerMaxCharger;							// time needed to react aggressive
			aggroOverTimerMax = aggroOverTimerMaxCharger;							// time that enemy stays aggro and tries to follow last position
			aggroRange = aggroRangeCharger;
			aggroHeight = aggroHeightCharger;
			bIsKickable = true;														// if enemy can be kicked by player
			speedValue = speedValueCharger;
			minRange = minRangeCharger;
			bCanReceiveCriticalHit = bCanReceiveCriticalHitCharger;
			fixedMovementAxis = fixedMovementAxisCharger;
			shieldActiveTimerMax = .0f;												// no shields for u!
			bCanEvade = bCanEvadeCharger;
			evadeCounter = evadeCounterCharger;
			evadeTimerMax = evadeTimerMaxCharger;
			evadeDistance = evadeDistanceCharger;
			evadeSpeed = evadeSpeedCharger;
			combatRange = combatRangeCharger;
			patrolSpeedMultiplier = patrolSpeedMultiplierCharger;
			shotFrequencyMax = shotFrequencyCharger;
			break;

		case EAttackType::EAT_range:
			healthMax = healthMaxShooter;
			aggroInitTimerMax = aggroInitTimerMaxShooter;							// time needed to react aggressive
			aggroOverTimerMax = aggroOverTimerMaxShooter;							// time that enemy stays aggro and tries to follow last position
			aggroRange = aggroRangeShooter;
			aggroHeight = aggroHeightShooter;
			bIsKickable = true;														// if enemy can be kicked by player
			speedValue = speedValueShooter;
			minRange = minRangeShooter;
			bCanReceiveCriticalHit = bCanReceiveCriticalHitShooter;
			fixedMovementAxis = fixedMovementAxisShooter;
			shieldActiveTimerMax = .0f;												// no shields for u!
			bCanEvade = bCanEvadeShooter;
			evadeCounter = evadeCounterShooter;
			evadeTimerMax = evadeTimerMaxShooter;
			evadeDistance = evadeSpeedShooter;
			evadeSpeed = evadeSpeedShooter;
			combatRange = combatRangeShooter;
			patrolSpeedMultiplier = patrolSpeedMultiplierShooter;
			shotFrequencyMax = shotFrequencyShooter;
			break;

		case EAttackType::EAT_defense:
			healthMax = healthMaxDefender;
			aggroInitTimerMax = aggroInitTimerMaxDefender;							// time needed to react aggressive
			aggroOverTimerMax = aggroOverTimerMaxDefender;							// time that enemy stays aggro and tries to follow last position
			aggroRange = aggroRangeDefender;
			aggroHeight = aggroHeightDefender;
			bIsKickable = false;													// if enemy can be kicked by player
			speedValue = speedValueDefender;
			minRange = minRangeDefender;
			bCanReceiveCriticalHit = bCanReceiveCrititcalHitDefender;
			fixedMovementAxis = fixedMovementAxisDefender;
			shieldActiveTimerMax = 5.f;
			defenderShieldHealthMax = 10.f;
			bCanEvade = bCanEvadeDefender;
			evadeCounter = evadeCounterDefender;
			evadeTimerMax = evadeTimerMaxDefender;
			evadeDistance = evadeDistanceDefender;
			evadeSpeed = evadeSpeedDefender;
			combatRange = combatRangeDefender;
			patrolSpeedMultiplier = patrolSpeedMultiplierDefender;
			shotFrequencyMax = shotFrequencyDefender;
			break;

		case EAttackType::EAT_mines:
			healthMax = healthMaxMines;
			aggroInitTimerMax = aggroInitTimerMaxMines;								// time needed to react aggressive
			aggroOverTimerMax = aggroOverTimerMaxMines;								// time that enemy stays aggro and tries to follow last position
			aggroRange = aggroRangeMines;
			aggroHeight = aggroHeightMines;
			bIsKickable = true;														// if enemy can be kicked by player
			speedValue = speedValueMines;
			minRange = minRangeMines;
			bCanReceiveCriticalHit = bCanReceiveCriticalHitMines;
			fixedMovementAxis = fixedMovementAxisMines;
			shieldActiveTimerMax = .0f;
			bCanEvade = bCanEvadeMines;
			evadeCounter = evadeCounterMines;
			evadeTimerMax = evadeTimerMaxMines;
			evadeDistance = evadeDistanceMines;
			evadeSpeed = evadeSpeedMines;
			combatRange = combatRangeMines;
			patrolSpeedMultiplier = patrolSpeedMultiplierMines;
			shotFrequencyMax = shotFrequencyMines;
			break;

		case EAttackType::EAT_walker:
			healthMax = healthMaxWalker;
			aggroInitTimerMax = aggroInitTimerMaxWalker;							// time needed to react aggressive
			aggroOverTimerMax = aggroOverTimerMaxWalker;							// time that enemy stays aggro and tries to follow last position
			aggroRange = aggroRangeWalker;
			aggroHeight = aggroHeightWalker;
			bIsKickable = true;														// if enemy can be kicked by player
			speedValue = speedValueWalker;
			minRange = minRangeWalker;
			bCanReceiveCriticalHit = bCanReceiveCriticalHitWalker;
			fixedMovementAxis = fixedMovementAxisWalker;
			shieldActiveTimerMax = .0f;												// no shields for u!
			bCanEvade = bCanEvadeWalker;
			evadeCounter = evadeCounterWalker;
			evadeTimerMax = evadeTimerMaxWalker;
			evadeDistance = evadeDistanceWalker;
			evadeSpeed = evadeSpeedWalker;
			combatRange = combatRangeWalker;
			patrolSpeedMultiplier = patrolSpeedMultiplierWalker;
			shotFrequencyMax = shotFrequencyWalker;
			break;
	}

	if (pawnSensingComp) {
		pawnSensingComp->HearingThreshold = aggroRange;
		pawnSensingComp->LOSHearingThreshold = aggroRange;
		pawnSensingComp->SightRadius = aggroRange;
	}

	// set player ref
	thePlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);


	// init vars 
	
	evadeTimerActual = 0.f;

	aggroInitTimerValue = 0.f;														// time since enemy can see player	
	aggroOverTimerValue = 0.f;
	if (!Cast<AUnholyCharacter>(thePlayer)->bGameIsLoaded) healthValue = healthMax;

	if (healthValue > 0) bIsAlive = true;
	else if (healthValue <= 0) bIsAlive = false;

	dashedTimerMax = .4f;
	dashedTimerValue = 0.f;
	bBeenDashed = false;

	bGotGrav = true;


	shotFrequencyValue = shotFrequencyMax;

	bBeenKicked = false;															// if enemy was kicked by player, reset in BP atm
	directionToPlayer = 0;															// 0: uh-oh, 1: left, 2: right
	bDisplayCritEffect = false;
	bPlayerIsInAggroRange = false;
	movementPattern = 0;															// 0: normal, 1: y only, 2: z only;
	inputValueX = 0.f;
	inputValueZ = 0.f;
	distanceToPlayer = 0.f;
	bDoFace = false;
	bDoExplode = false;

	lootTable = ELootTable::ELT_clean;

	allertRange = aggroRange;
	bIsAllerted = false;

	shieldActiveTimerValue = 0;

	if (attackType == EAttackType::EAT_defense) defenderShieldHealthValue = defenderShieldHealthMax;
	else defenderShieldHealthValue = 0.f;

	defenderShieldRegenTimerValue = 0.f;
	defenderShieldRegenValue = .2f;

	bIsInCombatRange = false;
	bIsFleeing = false;

	if (bDoesPatrol) {
		nonAggroState = ENonAggroState::ENA_patrol;
	}
	else {
		nonAggroState = ENonAggroState::ENA_idle;
	}

	patrolDirection = true;
	bIsPatroling = false;
}

void AEnemy::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (healthValue > healthMax) healthValue = healthMax;
	healthReturnValue = healthValue / healthMax;

	if (healthValue <= 0) {
		bIsAlive = false;
	}

	if (Cast<AUnholyCharacter>(thePlayer)->homingTarget == this) {
		bIsHomingTarget = true;
	}
	else {
		bIsHomingTarget = false;
	}
	
	// set speed value for anim
	movementSpeed = GetVelocity().Size();

	// check aggro range
	distanceToPlayer = (GetActorLocation() - thePlayer->GetActorLocation()).Size();

	if (distanceToPlayer < aggroRange) {
		bPlayerIsInAggroRange = true;
	}
	else {
		bPlayerIsInAggroRange = false;
	}

	if (pawnSensingComp) {
		if (pawnSensingComp->HasLineOfSightTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))) {
			if (bPlayerIsInAggroRange) bCanSeePlayer = true;
		}
	}

	// aggro reactions according to type
	if (bPlayerIsInAggroRange) {
		aggroOverTimerValue = 0.f;
		if (bCanSeePlayer) {
			if (aggroInitTimerValue < aggroInitTimerMax) {
				aggroInitTimerValue += DeltaTime;
			}
			else {
				if (nonAggroState == ENonAggroState::ENA_idle || nonAggroState == ENonAggroState::ENA_patrol) {
					nonAggroState = ENonAggroState::ENA_combat;
					aggroOrigin = GetActorLocation();
				}
				Aggro(thePlayer);
			}
		}
	}
	else if (bIsAllerted) {
		aggroOverTimerValue = 0.f;
		aggroInitTimerValue = aggroInitTimerMax;
		if (nonAggroState == ENonAggroState::ENA_idle || nonAggroState == ENonAggroState::ENA_patrol) {
			nonAggroState = ENonAggroState::ENA_combat;
			aggroOrigin = GetActorLocation();
		}
		Aggro(thePlayer);
	}
	else {
		if (aggroOverTimerValue < aggroOverTimerMax) {
			aggroOverTimerValue += DeltaTime;
		}
		else {
			// reset aggro
			aggroInitTimerValue = 0.f;
			aggroState = EAggroState::EAS_neutral;
			if (nonAggroState == ENonAggroState::ENA_combat) {
				if (bDoesPatrol) {
					nonAggroState = ENonAggroState::ENA_patrol;
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "pat");
				}
				else {
					nonAggroState = ENonAggroState::ENA_idle;
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "idle");
				}
			}
			// reset aim to fw
			SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
		}
		bAimWalkerChasis = false;

	}

	if (bBeenKicked && bIsAlive) {
		GetDistanceTo(thePlayer);
		float kickPower = Cast<AUnholyCharacter>(thePlayer)->baseKickPower;
		// pos direction: left, neg: right of player
		FVector distance = GetActorLocation() - thePlayer->GetActorLocation();			// actor loc - player loc
		FVector launchVector = FVector(0.f, 0.f, 0.f);
		// right
		if (distance.Y < 0) {
			launchVector.Y = -kickPower - (3 * distance.Y);
			if (launchVector.Y > 0) launchVector.Y = 0.f;
		}
		// left
		else if (distance.Y > 0) {
			launchVector.Y = kickPower - (3 * distance.Y);
			if (launchVector.Y < 0) launchVector.Y = 0.f;
		}
		LaunchCharacter(launchVector, true, false);
		bBeenKicked = false;
	}


	if (nonAggroState == ENonAggroState::ENA_idle) {
		bIsPatroling = false;
		bIsIntercepting = false;
		bIsFleeing = false;
		bGoTo = false;
	}
	else if (nonAggroState == ENonAggroState::ENA_patrol) {
		bIsPatroling = true;

		// set patrol speed
		GetCharacterMovement()->MaxFlySpeed = speedValue * patrolSpeedMultiplier;

		if (patrolDirection) {
			if (currentWayPoint < patrolWayPointLocations.Num()) {
				if (DistanceTo(patrolWayPointLocations[currentWayPoint]) > 50.f) {
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "moving to waypoint " + FString::FromInt(currentWayPoint) + " of " + FString::FromInt(patrolWayPointLocations.Num()) + "(" + FString::SanitizeFloat(DistanceTo(patrolWayPointLocations[currentWayPoint])) + ")");
					MovementToLocation(patrolWayPointLocations[currentWayPoint]);
				}
				else {
					currentWayPoint++;
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "next waypoint " + FString::FromInt(currentWayPoint));
				}
			}
			else if (currentWayPoint >= patrolWayPointLocations.Num()) {
				if (bPatrolReturn) {
					patrolDirection = false;
					currentWayPoint = currentWayPoint -2;
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "turning back >> " + FString::FromInt(currentWayPoint));
				}
				else {
					currentWayPoint = 0;
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "starting new");
				}
			}
		}
		else if (!patrolDirection) {
			if (currentWayPoint >= 0) {
				if (DistanceTo(patrolWayPointLocations[currentWayPoint]) > 50.f) {
					MovementToLocation(patrolWayPointLocations[currentWayPoint]); 
				}
				else {
					currentWayPoint--;
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, "last waypoint " + FString::FromInt(currentWayPoint));
				}
			}
			else if (currentWayPoint < 0) {
				if (bPatrolReturn) {
					patrolDirection = true;
					currentWayPoint = 0;
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "turning normal >> " + FString::FromInt(currentWayPoint));
				}
			}
		}
	}
	else if (nonAggroState == ENonAggroState::ENA_combat) {
		bIsPatroling = false;
		/*
		// if actor is in hitactors(playerlist),
		if (aggroState != EAggroState::EAS_neutral) {
			for (int i = 0; i < Cast<AUnholyCharacter>(thePlayer)->hitActors.Num(); i++) {
				// do evade
				if (Cast<AUnholyCharacter>(thePlayer)->hitActors[i] == this) {
					Cast<AUnholyCharacter>(thePlayer)->hitActors.RemoveAt(i);
					bIsEvading = true;
					originBeforeEvade = GetActorLocation();
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "EY!!");
				}
			}
		}

		if (bIsEvading) {
			float evadedDistance = (originBeforeEvade - GetActorLocation()).Size();
			GetCharacterMovement()->MaxFlySpeed = 600 * evadeSpeed;
			FVector moveTo = GetActorLocation();
			moveTo.Z = moveTo.Z + evadeDistance;
			bGoTo = true;
			MovementToLocation(moveTo);
		
			if (evadedDistance > evadeDistance ) {
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::SanitizeFloat(DistanceTo(moveTo)));
				bGoTo = false;
				bIsEvading = false;
			}
		}
		else {*/

		// increase speed while fleeing
		if (bIsFleeing) {
			GetCharacterMovement()->MaxFlySpeed = speedValue *2;
		}
		else {
			if (attackType == EAttackType::EAT_defense) {
				if (bIsIntercepting) {
					speedValue = speedValueDefender * .5f;
				}
				speedValue = speedValueDefender;	
			}
			GetCharacterMovement()->MaxFlySpeed = speedValue;
		}
		
		// defender shield stuff
		if (attackType == EAttackType::EAT_defense) {
			if (defenderShieldRegenTimerValue > defenderShieldRegenTimerMax) {
				if (defenderShieldHealthValue < defenderShieldHealthMax) {
					defenderShieldHealthValue += defenderShieldRegenValue;
				}
			}
			else {
				defenderShieldRegenTimerValue += DeltaTime;
			}
			if (defenderShieldHealthValue <= 0) {
				minRange = minRangeCharger;
				attackType = EAttackType::EAT_charge;
			}
		}

		// set new speed incase charger switch
		if (attackType == EAttackType::EAT_charge) {
			GetCharacterMovement()->MaxFlySpeed = speedValueCharger;
		}
		//}
	}
}

// ### BEHAVIOUR ###

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!bIsAlive) return;

	UPrimitiveComponent* overlappedComp = SweepResult.GetComponent();
	FString hitComp = overlappedComp->GetName();

	// if hit by knockback
	if (OtherActor->GetName().Contains("knockBackSphere")) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, ":: " + OtherActor->GetName());
		if (bIsKickable) {
			bBeenKicked = true;
		}
	}
	else {
		// if player gets into range (comming from BP)
		if (hitComp == "aggroRange") {
			if (!bCanSeePlayer) return;
			FString returno = overlappedComp->GetName();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "icu!");
			this->aggroState = EAggroState::EAS_attack;
			bPlayerIsInAggroRange = true;
		}
		// if headshot
		if (hitComp == "headTriggerVolume" && OtherActor->GetName().Contains("PlayerBullet")) {
			if (bCanReceiveCriticalHit) {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "critical hit!");
				bDisplayCritEffect = true;   // reset in BP
				bBeenCrit = true;
			}
			else {
				bBeenHit = true;
			}
			theHitComp = OtherComp;
			theHitMan = OtherActor;
			bIsAllerted = true;
			this->aggroState = EAggroState::EAS_attack;
		}
		// if bodyhit by player bullet (comming from cpp)
		if (hitComp == "bodyTriggerVolume") {
			bBeenHit = true;
			theHitComp = OtherComp;
			theHitMan = OtherActor;
			bIsAllerted = true;
			this->aggroState = EAggroState::EAS_attack;
		}
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	if (!OtherActor->GetName().Contains("KnockBackSphere")) {
		if (OtherActor->ActorHasTag("Player")) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "nocu!");
			//aggroTimerValue = 0.f;
			bPlayerIsInAggroRange = false;
			bCanShoot = false;
			bCanSeePlayer = false;
		}
	}
}

void AEnemy::OnSeePlayer(APawn* Pawn) {
	if (!bIsAlive) return;

	//AEnemy_AIController* AIController = Cast<AEnemy_AIController>(GetController());
	//Set the seen target on the blackboard
	bCanSeePlayer = true;
	if (distanceToPlayer < aggroRange) {
		bPlayerIsInAggroRange = true;
	}
	//if (AIController)
	//{
	//	AIController->SetSeenTarget(Pawn);
	//}
}

void AEnemy::Aggro(AActor* target) {
	if (!bIsAlive) return;
	nonAggroState = ENonAggroState::ENA_combat;
	lastKnownPlayerPosition = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();

	bGoTo = false;

	switch (attackType) {
	case EAttackType::EAT_range:
		aggroState = EAggroState::EAS_attack;
		AimChasis();
		// check range of player, if out of minRange, move to
		if (GetVerticalDistanceTo(thePlayer) > (minRange) || GetHorizontalDistanceTo(thePlayer) > (minRange)) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "+");
			bGoTo = false;
			MovementToLocation(lastKnownPlayerPosition);
		}
		// else if closer than half minRange, get away from (to minRange)
		else if (GetVerticalDistanceTo(thePlayer) < (minRange - combatRange) || GetHorizontalDistanceTo(thePlayer) < (minRange - combatRange)) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "-");
			bIsFleeing = true;
			bGoTo = true;
		}
		else {
			bGoTo = false;
		}
		ShooterAttack(target);
		break;

	case EAttackType::EAT_allert:
		aggroState = EAggroState::EAS_attack;
		AimChasis();
		bDoFace = true;
		// check range of player, if out of minRange, move to

		if (!bIsInCombatRange && !bIsFleeing) {
			if (distanceToPlayer > minRange) {
				bGoTo = false;
				MovementToLocation(lastKnownPlayerPosition);
			}
			else {
				bIsInCombatRange = true;
			}
		}
		else {
			if (distanceToPlayer < minRange - combatRange) {
				bGoTo = true;
				bIsFleeing = true;
			}
			else if (distanceToPlayer >= minRange) {
				bIsFleeing = false;
			}
		}
		if (distanceToPlayer > minRange + combatRange) {
			bIsInCombatRange = false;
		}

		AllerterAttack(lastKnownPlayerPosition);
		break;

	case EAttackType::EAT_charge:
		aggroState = EAggroState::EAS_attack;
		AimChasis();
		MovementToLocation(lastKnownPlayerPosition);
		bIsIntercepting = false;
		bGoTo = false;
		fixedMovementAxis = 0;
		if (distanceToPlayer <= (minRange + 20)) {
			ChargerAttack();
		}
		break;

	case EAttackType::EAT_defense:
		// move to min range and block
		aggroState = EAggroState::EAS_defense;
		AimChasis();

		if (!bIsInCombatRange && !bIsFleeing) {
			// move to combat range
			if (distanceToPlayer > minRange + combatRange) {
				bGoTo = false;
				MovementToLocation(lastKnownPlayerPosition);
				bShowLOSPos = false;
				bIsIntercepting = false;
				fixedMovementAxis = 0;
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "pos");
			}
			else {
				bIsInCombatRange = true;
				bShowLOSPos = false;
				bIsIntercepting = false;
			}
		}
		else {
			if (distanceToPlayer < minRange - combatRange) {
				bGoTo = true;
				bIsFleeing = true;
				bShowLOSPos = false;
				bIsIntercepting = false;
				fixedMovementAxis = 0;
			}
			else if (!bIsFleeing) {
				// intercept
				if (Cast<AUnholyCharacter>(thePlayer)->bShowDebugWindow) bShowLOSPos = true;
				else bShowLOSPos = false;

				// check if player is facing enemy, if not, normal behaviour
				if (Cast<AUnholyCharacter>(thePlayer)->face == "R") {
					if (GetDirectionToPlayer() == 1) {
						// if rotation.yaw = 90, intercept
						bGoTo = true;
						MovementToLocation(intersectionLOS);
						//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "los (L)");
						bIsIntercepting = true;
						fixedMovementAxis = 1;
					}
				}
				else if (Cast<AUnholyCharacter>(thePlayer)->face == "L") {
					// if rotation.yaw = -90, intercept
					if (GetDirectionToPlayer() == 2) {
						bGoTo = true;
						MovementToLocation(intersectionLOS);
						//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "los (R)");
						bIsIntercepting = true;
						fixedMovementAxis = 1;
						}
				}
				else {
					bIsIntercepting = false;
					fixedMovementAxis = 0;
				}
			}

			if (distanceToPlayer >= minRange) {
				bIsFleeing = false;
			}

			if (bIsFleeing) {
				movementPattern = 1;
				FleeYouFewl();

			}
		}
		if (distanceToPlayer > minRange + combatRange) {
			bIsInCombatRange = false;
		}

	case EAttackType::EAT_walker:
		aggroState = EAggroState::EAS_attack;
		AimWalkerChasis();
		// check range of player, if out of minRange, move to
		if (GetHorizontalDistanceTo(thePlayer) > (minRange)) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "+");
			bGoTo = false;
			fixedMovementAxis = fixedMovementAxisWalker;
			MovementToLocation(lastKnownPlayerPosition);
			bWalkerIsShooting = false;
		}
		// else if closer than half minRange, get away from (to minRange)
		else if (GetHorizontalDistanceTo(thePlayer) < (minRange - combatRange)) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "-");
			bIsFleeing = true;
			bGoTo = true;
			bWalkerIsShooting = false;
		}
		else {
			bGoTo = false;
			ShooterAttack(target);		// replace with walker attack!!!!!!!
		}

		if (distanceToPlayer >= minRange) {
			bIsFleeing = false;
		}

		if (bIsFleeing) {
			movementPattern = fixedMovementAxisWalker;
			FleeYouFewl();
		}
		break;
	}
}


void AEnemy::FleeYouFewl() {
	AUnholyCharacter* theCharacter = Cast<AUnholyCharacter>(thePlayer);
	FRotator theAimRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), theCharacter->GetCharacterCenter());
	FVector playerLoc = theCharacter->GetActorLocation();
	FVector ownLocation = GetActorLocation();

	// not fleeing to the right.... huaaaaai!? differ between L & R?
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "---1");
	FVector differenceVector = ownLocation - playerLoc;
	reachLoc = ownLocation + differenceVector;
	fixedMovementAxis = 1;
	MovementToLocation(reachLoc);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "reachloc (0)");
}

float AEnemy::DistanceTo(FVector location) {
	float distance = 0;
	distance = (GetActorLocation() - location).Size();
	return distance;
}

// DOES NOT SEEM TO WORK ON NEW SUBCLASSES! HUAAAAI?! FIX!
void AEnemy::AllerterAttack(FVector pPosition) {
	if (!bIsAlive) return;

	// get all allies in range
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), alliedEnemiesInRange);
	int allertedAllies = 0;
	for (int i = 0; i < alliedEnemiesInRange.Num(); i++) {
		// check allert range	
		if (DistanceTo(alliedEnemiesInRange[i]->GetActorLocation()) <= allertRange)  { // && Cast<AEnemy>(alliedEnemiesInRange[i])->bIsAlive
			// allert ally
			//if (Cast<AEnemy>(alliedEnemiesInRange[i])->bCanSeePlayer) {
			Cast<AEnemy>(alliedEnemiesInRange[i])->bIsAllerted = true;
			// hand over player loc
			Cast<AEnemy>(alliedEnemiesInRange[i])->lastKnownPlayerPosition = pPosition;
			allertedAllies++;
			//}
			
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(allertedAllies) + " allies allerted!");
}

// moves to minRange, and attacks target when reached 
void AEnemy::ShooterAttack(AActor* target) {
	if (!bIsAlive) return;

	if (shotFrequencyValue >= shotFrequencyMax) {
		bCanShoot = true;
	}
	else {
		shotFrequencyValue += GetWorld()->GetDeltaSeconds();
	}
}

void AEnemy::WalkerAttack(AActor* target) {
	if (!bIsAlive) return;

	bWalkerIsShooting = true;
	/*if (shotFrequencyValue >= shotFrequencyMax) {
		bCanShoot = true;
	}
	else {
		shotFrequencyValue += GetWorld()->GetDeltaSeconds();
	}*/
}

void AEnemy::ChargerAttack() {
	if (healthValue > 0) {
		bDoExplode = true;
	}
}

void AEnemy::Defend() {
	if (distanceToPlayer <= minRange) {
		bActivateShield = true;
	}
	else {
		if (shieldActiveTimerValue < shieldActiveTimerMax) {
			shieldActiveTimerValue += GetWorld()->GetDeltaSeconds();
		}
		else {
			bActivateShield = true;
		}
	}
}

int AEnemy::DirectionToLocation(FVector location) {
	int returnValue = 0;
	
	FRotator theAimRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), location);
	if (GetActorLocation().Y < location.Y) {
		// actor facing left
		returnValue = 1;
		theAimRotation.Yaw = 90.f;
	}
	else if (GetActorLocation().Y > location.Y) {
		// actor facing right
		returnValue = 2;
		theAimRotation.Yaw = -90.f;
	}
	else {
		returnValue = 0;
	}
	
	return returnValue;
}

int AEnemy::GetDirectionToPlayer() {
	int direction;

	if (GetActorLocation().Y < thePlayer->GetActorLocation().Y) {
		// left
		direction = 1;
		directionToPlayer = 1;
	}
	else if (GetActorLocation().Y > thePlayer->GetActorLocation().Y) {
		// right
		direction = 2;
		directionToPlayer = 2;
	}
	else {
		direction = 0;
		directionToPlayer = 0;
	}

	return direction;
}

void AEnemy::AimChasis() {
	if (!bIsAlive) return;

	AUnholyCharacter* theCharacter = Cast<AUnholyCharacter>(thePlayer);
	theAimRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), theCharacter->GetCharacterCenter());
	if (GetActorLocation().Y < thePlayer->GetActorLocation().Y) {
		// left
		directionToPlayer = 1;
		theAimRotation.Yaw = 90.f;
	}
	else if (GetActorLocation().Y > thePlayer->GetActorLocation().Y) {
		// right
		directionToPlayer = 2;
		theAimRotation.Yaw = -90.f;
	}
	else {
		directionToPlayer = 0;
	}
	SetActorRotation(theAimRotation);
}

void AEnemy::AimWalkerChasis(){
	if (!bIsAlive) return;

	bAimWalkerChasis = true;
	AUnholyCharacter* theCharacter = Cast<AUnholyCharacter>(thePlayer);
	theAimRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), theCharacter->GetCharacterCenter());
	if (GetActorLocation().Y < thePlayer->GetActorLocation().Y) {
		// left
		directionToPlayer = 1;
		theAimRotation.Yaw = 90.f;
	}
	else if (GetActorLocation().Y > thePlayer->GetActorLocation().Y) {
		// right
		directionToPlayer = 2;
		theAimRotation.Yaw = -90.f;
	}
	else {
		directionToPlayer = 0;
	}
	walkerChasisRotation = theAimRotation;
}

void AEnemy::AimWeapon() {
	if (!bIsAlive) return;

	// !!! only used for special enemies
}

// moves to location till minRange reached, checks for blocks
void AEnemy::MovementToLocation(FVector destination) {
	if (!bIsAlive) return;

	if (!bIsFleeing && !bIsIntercepting && !bIsPatroling) movementPattern = CheckPathToTarget();
	distanceToLocation = (GetActorLocation() - destination).Size();
	
	if (bIsIntercepting) {
		if (distanceToLocation > 75.f) {
			// (1) need to compare destination with actorloc
			float actorValueY = GetActorLocation().Y;
			float actorValueZ = GetActorLocation().Z;
			float destinationY = destination.Y;
			float destinationZ = destination.Z;

			// (2) find out how much the value is between x and y (1:x)
			float destinationValueY = actorValueY - destinationY;
			float destinationValueZ = actorValueZ - destinationZ;

			float value = destinationValueY / destinationValueZ;
			valueY = 1;
			valueZ = 1 / value;

			if (DirectionToLocation(destination) == 1) {
				//AddMovementInput(horizonCube->GetActorForwardVector(), valueY * speedValue);
				AddMovementInput(horizonCube->GetActorUpVector(), valueZ * speedValue);
			}
			else if (DirectionToLocation(destination) == 2) {
				//AddMovementInput(horizonCube->GetActorForwardVector(), -valueY * speedValue);
				AddMovementInput(horizonCube->GetActorUpVector(), -valueZ * speedValue);
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "movement pattern 2i: 0");
			}
		}
	}
	else {
		// (1) need to compare destination with actorloc
		float actorValueY = GetActorLocation().Y;
		float actorValueZ = GetActorLocation().Z;
		float destinationY = destination.Y;
		float destinationZ = destination.Z;

		// (2) find out how much the value is between x and y (1:x)
		float destinationValueY = actorValueY - destinationY;
		float destinationValueZ = actorValueZ - destinationZ;
		float value = destinationValueY / destinationValueZ;
		valueY = 1;
		valueZ = 1 / value;																				// that's the bloody reason l/r go -1 on z suddenly

		// (3) move
		if (movementPattern == 0) {
			// check horizontal and vertical dist vs range

			if (DirectionToLocation(destination) == 1) {
				AddMovementInput(horizonCube->GetActorForwardVector(), valueY * speedValue);
				AddMovementInput(horizonCube->GetActorUpVector(), valueZ * speedValue);
			}
			else if (DirectionToLocation(destination) == 2) {
				AddMovementInput(horizonCube->GetActorForwardVector(), -valueY * speedValue);
				AddMovementInput(horizonCube->GetActorUpVector(), -valueZ * speedValue);
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "movement pattern 0: 0");
			}
		}
		else if (movementPattern == 1) {
			// ## check horizontal distance vs range

			if (DirectionToLocation(destination) == 1) {
				AddMovementInput(horizonCube->GetActorForwardVector(), valueY * speedValue);
			}
			else if (DirectionToLocation(destination) == 2) {
				AddMovementInput(horizonCube->GetActorForwardVector(), -valueY * speedValue);
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "movement pattern 1: 0");
			}
		}
		else if (movementPattern == 2) {
			// ## check vertical distance vs range
			if (DirectionToLocation(destination) == 1) {
				//AddMovementInput(horizonCube->GetActorForwardVector(), valueY * speedValue);
				AddMovementInput(horizonCube->GetActorUpVector(), valueZ * speedValue);
			}
			else if (DirectionToLocation(destination) == 2) {
				//AddMovementInput(horizonCube->GetActorForwardVector(), -valueY * speedValue);
				AddMovementInput(horizonCube->GetActorUpVector(), -valueZ * speedValue);
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "movement pattern 2: 0");
			}
		}
	}
	
}

int AEnemy::CheckPathToTarget() {
	if (!bIsAlive) return 0;
	if (bGoTo) return 1;												// while fleeing, stay on y		
	if (fixedMovementAxis != 0) return fixedMovementAxis;				// atm only allerters are fixed

	// returns movement direction;
	// 0: wrong return
	// 1: move y only
	// 2: move z only

	int directionToMove = 0;
	int direction = 0;

	if (GetActorLocation().Z > thePlayer->GetActorLocation().Z) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "down");
		direction = 1;
	}
	else if (GetActorLocation().Z < thePlayer->GetActorLocation().Z) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "up");
		direction = 2;
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "fw");
		direction = 0;
	}

	FVector actorLocUp = upperMeshBorder->GetComponentLocation();
	FVector actorLocLow = lowerMeshBorder->GetComponentLocation();
	FVector Start = GetActorLocation();

	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			Start = actorLocLow;
		}
		else if (i == 1) {
			Start = actorLocUp;
		}

		FVector ForwardVector = GetActorForwardVector();
		FVector End = GetActorLocation();
		if (i == 0) {
			End = (ForwardVector * distanceToPlayer) + Start;
		}
		else if (i == 1) {
			End = (ForwardVector * distanceToPlayer) + Start;
		}

		// additional trace parameters
		FCollisionQueryParams CollisionParams;
		CollisionParams.bTraceComplex = true;
		CollisionParams.AddIgnoredComponent(enemyModel_static);
		CollisionParams.AddIgnoredComponent(enemyModel_weapon);
		CollisionParams.AddIgnoredComponent(headTriggerVolume);
		CollisionParams.AddIgnoredComponent(model_destruct);
		CollisionParams.AddIgnoredActor(this);

		//Re-initialize hit info
		FHitResult HitDetails = FHitResult(ForceInit);
		if (GetWorld()->LineTraceSingleByChannel(HitDetails, Start, End, ECC_PhysicsBody, CollisionParams)) {

			FString returnString = HitDetails.Actor->GetName();
			// debuggeronie
			if (Cast<AUnholyCharacter>(thePlayer)->bShowDebugWindow) {
				if (i == 0) {
					DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, ECC_WorldStatic, .5f);
				}
				else if (i == 1) {
					DrawDebugLine(GetWorld(), Start, End, FColor::White, false, 1.f, ECC_WorldStatic, .5f);
				}
			}

			// (move down)
			if (direction == 1) {
				if (i == 0) {
					// if lower hits level, move y only
					if (HitDetails.Actor != thePlayer) {
						directionToMove = 1;
					}
				}
				else if (i == 1) {
					// if upper hits level, move z only
					if (HitDetails.Actor != thePlayer) {
						directionToMove = 2;
					}
				}

			}
			// (move up)
			else if (direction == 2) {
				if (i == 1) {
					// if upper hits level, move y only
					if (HitDetails.Actor != thePlayer) {
						directionToMove = 1;
					}
				}
				else if (i == 0) {
					// if lower hits level, move z only
					if (HitDetails.Actor != thePlayer) {
						directionToMove = 2;
					}
				}
			}
		}
	}

	return directionToMove;
}

