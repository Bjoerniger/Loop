// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UnholyCharacter.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AUnholyCharacter::AUnholyCharacter() {
	// Set size for collision capsule
	//GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(-10.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;

	gScale = 2.f;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // don't face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = gScale;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 550.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	GetCharacterMovement()->MaxAcceleration = 6000.f;					// high value to hide acceleration
	GetCharacterMovement()->BrakingDecelerationWalking = 6000.f;		// high value to hide decelaration

	// varrying character vars
	maxMovementSpeed = GetCharacterMovement()->MaxWalkSpeed;
	backwardsSpeedModifier = .75f;
	runningSpeedModifier = 1.f;
	jumpCounterMax = 1;
	
	bIsOnGround = false;
	bIsJumping = false;

	pitch = 0.f;

	relativeMouseLocX = 0.f;
	relativeMouseLocY = 0.f;

	weaponOffsetY = 0.f;
	pitchX = 0.f;
	pitchY = 0.f;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bAllowMovement = false;
	bCanClimb = false;

	bUsableInRange = false;
	bUsableActivated = false;
	ActivateObjectName = "";

	dashTimerActive = 0.f;
	dashTimerMax = 2.f;						// time between double move to dash

	dashCDMax = 3.f;						// time between 2 dashes
	dashCDActive = dashCDMax;
	dashSpeedMultiplicator = 200.f;
	canDash = true;

	isDashing = false;
	dashingTimeMax = .35f;					// time a dash takes
	dashingTimeActive = 0.f;
	launchSpeed = 3500.f;

	doubleJumpLaunchPowerY = 700.f;			// length
	doubleJumpLaunchPowerZ = 800.f;			// height

	bIsAtWallJump = false;
	bCanWallJump = false;

	bShowOnCharUI = false;

	healthMax = 10.f;
	healthValue = healthMax;

	bDownPressed = false;

	skills_CanWallJump = false;
	skills_CanDash = false;

	jumpType = EJumpType::EJT_clean;
	jumpPowerValue = 1.f;
	jumpPowerMax = 1.25f;

	bDoStomp = false;
	bIsStomping = false;

	stompTimerValue = 5.f;
	stompTimerMax = 5.f;					// max value for dmg increase by height @stomp
	stompSpeed = 2.5f;						// gravity multiplyer during stomp
	stompRdy = true;
	stompCoolDownMax = 5.f;
	stompCoolDownValue = 0.f;
	stompCoolDown = true;

	bShieldActive = false;
	shieldChargesMax = 0;
	shieldChargesValue = shieldChargesMax;
	shieldRechargeTimerMax = 3.f;
	shieldRechargeTimerValue = .0f;
	timeSinceLastShieldHitMax = 5.f;		// time it takes till shields start recharging after hit
	timeSinceLastShieldHitValue = .0f;		// time since shield last blocked a hit

	chargeFireTimerMax = 5.f;				// time between charge shots
	chargeFireTimerValue = 0.f;
	chargeActiveTimerMax = 1.0f;			// time it takes to charge the shot
	chargeActiveTimerValue = 0.f;
	bCanChargeFire = true;

	bSkillMenuDisplayed = false;
	skillPoints = 1;
	
	skills_AdvancedJump = false;
	skills_CanDash = false;
	skills_CanStomp = false;
	skills_CanWallJump = false;
	skills_CanChargeFire = false;

	skillLevel_health = 0;
	skillLevel_dashCD = 0;
	skillLevel_wallhugDuration = 0;
	skillLevel_StompCD = 0;
	skillLevel_chargeUpShotTime = 0;
	skillLevel_shieldRechargeTime = 0;
	skillLevel_shieldRechargeDuration = 0;
	skillLevel_gadgetCoolDown = 0;
	skillLevel_GadgetCapacity = 0;

	// the value a stat increases per level, still WiP, 1.f as placeholder
	skillValue_health = 1.f;
	skillValue_dashCD = 1.f;
	skill_ValuewallhugDuration = 1.f;
	skillValue_StompCD = 1.f;
	skillValue_chargeUpShotTime = 1.f;
	skillValue_shieldRechargeTime = 1.f;
	skillValue_shieldRechargeDuration = 1.f;
	skillValue_gadgetCoolDown = 1.f;
	skillValue_GadgetCapacity = 1.f;

	// weapon stats
	bulletDamage = 3.f;
	bulletRange = .5f;
	bulletSpeed = 2000.f;
	bulletPenetration = 1;
	//chargeSpeed == chargeActiveTimerMax;
	chargeReadyTime = 2.f;							// time a special bullet charge stais rdy to use
	critMultiplier = 2.f;

}

// Input
void AUnholyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	// set up gameplay key bindings
	if (!bAllowMovement) return;

	else {
		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUnholyCharacter::SpaceDown);
		//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &AUnholyCharacter::SpaceUp);
		PlayerInputComponent->BindAxis("MoveRight", this, &AUnholyCharacter::MoveRight);
		PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AUnholyCharacter::Dash);
		PlayerInputComponent->BindAction("MouseLeft", IE_Pressed, this, &AUnholyCharacter::FireWeapons);
		PlayerInputComponent->BindAction("MouseRight", IE_Pressed, this, &AUnholyCharacter::FireCharged);
		PlayerInputComponent->BindAction("MouseRight", IE_Released, this, &AUnholyCharacter::FireChargeActivated);
		PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AUnholyCharacter::Use);
		PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AUnholyCharacter::DownPressed);
		PlayerInputComponent->BindAction("Down", IE_Released, this, &AUnholyCharacter::DownReleased);
	}
	
}

void AUnholyCharacter::Tick(float deltaTime) {
	Super::Tick(deltaTime);
	
	if (bAllowMovement) {
		GetMousePos();
		GetAim();
		IncreaseSkillsByLevel();

		// set jump power
		if (space) {
			if (jumpPowerValue < jumpPowerMax) {
				jumpPowerValue += deltaTime;
			}
			else {
				jumpPowerValue = jumpPowerMax;
			}
		}

		// check for usable
		if (!bUsableInRange) {
			bUsableActivated = false;
		}

		// reset jump stuff on ground
		if (GetCharacterMovement()->IsMovingOnGround()) {
			if (jumpType != EJumpType::EJT_clean) {
				jumpType = EJumpType::EJT_clean;
			}
			bIsOnGround = true;
			AdjustMovementSpeed();
			jumpCounter = 0;
			bIsFalling = false;
			bDoStomp = false;
		}
		else {
			bIsOnGround = false;
			if (GetSpeed(3) < 0) {
				bIsFalling = true;
			}
			else {
				bIsFalling = false;
			}
		}

		// ## set rotation
		// ground: to mouse
		if (bIsOnGround) {
			facingMethod = 1;
			SetCharacterRotationToMouse();
		}
		// wallhug: face wallNormal
		else if (bIsAtWallJump) {
			facingMethod = 2;
			FRotator rot = jumpDirection.Rotation();
			SetActorRotation(rot);
		}
		else if (!bIsOnGround) {
			// jump
			if (jumpType == EJumpType::EJT_single) {
				facingMethod = 3;	
				SetCharacterRotationToMouse();
			}
			// double jump
			else if (jumpType == EJumpType::EJT_double) {
				facingMethod = 4;
				SetCharacterRotationToMouse();
			}
			FRotator rot = jumpDirection.Rotation();
			if (face == "L") {
				rot.Yaw = 90;
			}
			else if (face == "R") {
				rot.Yaw = -90;
			}
			SetActorRotation(rot);

			// on DJ reset jumptype to single, to stop rotation after initial possible turn
			if (jumpType == EJumpType::EJT_double) {
				jumpType = EJumpType::EJT_single;
			}
		}
		else {
			facingMethod = 0;
		}

		if (isDashing) {
			dashingTimeActive += deltaTime;
			if (dashingTimeActive >= dashingTimeMax) {
				isDashing = false;
			}
		}
		else {
			// reset speed
			AdjustMovementSpeed();
			if (!canDash) {
				if (dashCDActive <= dashCDMax) {
					dashCDActive += deltaTime;
				}
				else {
					canDash = true;
				}
			}

			if (dashed) {
				if (dashTimerActive < dashCDDecreased) {
					dashTimerActive += deltaTime;
				}
				else {
					launchVector = FVector(0.f, 0.f, 0.f);
					dashTimerActive = 0.f;
					dashed = false;
				}
			}

			// wallhug
			if (bIsAtWallJump) {
				GetCharacterMovement()->GravityScale = 0.f;
				GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
			}
			else if (bIsStomping || bDoStomp) {
				// add stomp power
				if (stompTimerValue <= stompTimerMax) {
					stompRdy = false;
					stompTimerValue += deltaTime;
				}
				else {
					stompRdy = true;
				}
				// raise gravity for stomp
				GetCharacterMovement()->GravityScale = gScale * stompSpeed;
			}
			else if (!bIsStomping && !bDoStomp) {
				// reset gravity after stomp
				GetCharacterMovement()->GravityScale = gScale;
			}
			if (bIsOnGround && bIsJumping)
			{
				bIsJumping = false;
			}
		}

		// check shield recharge
		if (shieldChargesValue < shieldChargesMax) {
			// check if recharge is rdy
			if (timeSinceLastShieldHitValue <= timeSinceLastShieldHitMaxDecreased) {
				timeSinceLastShieldHitValue += deltaTime;
			}
			else {
				if (shieldRechargeTimerValue <= shieldRechargeTimerMaxDecreased) {
					shieldRechargeTimerValue += deltaTime;
				}
				else {
					shieldChargesValue += 1;
					shieldRechargeTimerValue = 0.f;
				}
			}
		}

		// stomp cooldown
		if (!stompCoolDown) {
			if (stompCoolDownValue < stompCoolDownMax) {
				stompCoolDownValue += deltaTime;
			}
			else {
				stompCoolDownValue = 0.f;
				stompCoolDown = true;
			}
		}

		// charge fire timer
		if (chargeFireTimerValue < chargeFireTimerMaxDecreased) {
			chargeFireTimerValue += deltaTime;
		}
		else {
			bCanChargeFire = true;
		}

		if (bIsCharging) {
			if (chargeActiveTimerValue < chargeActiveTimerMax) {
				chargeActiveTimerValue += deltaTime;
			}
		}
	}
}

void AUnholyCharacter::GetMousePos() {
	// Get Screen Size
	GEngine->GameViewport->GetViewportSize(screenSize);
	screenSizeX = screenSize.X;
	screenSizeY = screenSize.Y;
	// get Mouse Position
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController) playerController->GetMousePosition(mouseLocationX, mouseLocationY);
	// set relative Mouse Position
	relativeMouseLocX = mouseLocationX - screenSizeX / 2;
	relativeMouseLocY = ((mouseLocationY - weaponOffsetY) - screenSizeY / 2) * -1.f;
}

// Set Character Rotation according to Mouse Position
void AUnholyCharacter::SetCharacterRotationToMouse() {
	if (!bAllowMovement) return;
	else {
		// Check if Mouse Left or Right of Character when on ground
		FRotator rot = GetActorRotation();
		if (mouseLocationX > screenSizeX / 2) {
			face = "R";
			rot.Yaw = -90;
		}
		else if (mouseLocationX < screenSizeX / 2) {
			face = "L";
			rot.Yaw = 90;
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "not sure what you just did, but you fucked the char rotation");
		}
		SetActorRotation(rot);		
	}
}

// sets pitch of upper body / aim
void AUnholyCharacter::GetAim() {
	
	// check if aim is above, below or mid
	float ang;

	if (relativeMouseLocX > 0) {
		ang = FMath::Atan2(relativeMouseLocX, relativeMouseLocY);
	}
	else {
		ang = FMath::Atan2(-relativeMouseLocX, relativeMouseLocY);
	}

	float maxPitchValue = 90.f;
	pitch = FMath::RadiansToDegrees(ang);								// returns a value between 0 (up) and 180 (down)
	if (pitch < maxPitchValue) pitch = maxPitchValue - pitch;						// values recalculated to 90 (up) and -90 (down)
	else if (pitch >= maxPitchValue) pitch = -(pitch - maxPitchValue);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::SanitizeFloat(pitch));
	}
	
void AUnholyCharacter::Dash() {
	if (!skills_CanDash) return;

	// dont dash during dashing
	if (dashed) return;
	
	// dont dash when cd not rdy
	if (!canDash) return;

	else {
		launchVector = FVector(0.f, 0.f, 0.f);
		// do dash
		if (inputValueX > 0) {						// right
			if (face == "R") {
				dashed = true;
				isDashing = true;
				dashCDActive = 0.f;
				dashingTimeActive = 0.f;
				canDash = false;
				launchVector = FVector(0.f, -launchSpeed, 0.f);
			}
			else if (face == "L") {
				dashed = true;
				isDashing = true;
				dashCDActive = 0.f;
				dashingTimeActive = 0.f;
				canDash = false;
				launchVector = FVector(0.f, -launchSpeed, 0.f);
			}
		}
		else if (inputValueX < 0) {					// left
			if (face == "L") {
				dashed = true;
				isDashing = true;
				dashCDActive = 0.f;
				dashingTimeActive = 0.f;
				canDash = false;
				launchVector = FVector(0.f, launchSpeed, 0.f);
			}
			else if (face == "R") {
				dashed = true;
				isDashing = true;
				dashCDActive = 0.f;
				dashingTimeActive = 0.f;
				canDash = false;
				launchVector = FVector(0.f, launchSpeed, 0.f);
			}
		}

		if (bIsJumping || !bIsOnGround || bIsFalling) {
			// need to fix launch vector for jumps (too fast!)
			FVector launchVectorJump = launchVector / 6.f;
			LaunchCharacter(launchVectorJump, false, false);
		}
		else {
			LaunchCharacter(launchVector, true, false);
		}
		


	}
}

void AUnholyCharacter::SpaceDown() {
	if (!bAllowMovement) return;
	space = true;

	// if walljump
	if (bCanWallJump) {
		// if walljump skill not aquired, piss off!
		if (!skills_CanWallJump) return;

		bIsAtWallJump = true;
		jumpCounter = 0;
		bIsJumping = false;
	}
	
	
}

void AUnholyCharacter::SpaceUp() {
	if (!bAllowMovement) return;
	else {
		space = false;

		// # wall jump
		if (bIsAtWallJump) {	
			if (!bDownPressed) {
				bIsAtWallJump = false;
				jumpCounter = 0;
				if (jumpDirection.Y > 0) {
					face = "R";
					LaunchCharacter(FVector(0, -doubleJumpLaunchPowerY, doubleJumpLaunchPowerZ * jumpPowerValue), false, true);
					jumpType = EJumpType::EJT_wall;
				}
				else if (jumpDirection.Y < 0) {
					face = "L";
					LaunchCharacter(FVector(0, doubleJumpLaunchPowerY, doubleJumpLaunchPowerZ * jumpPowerValue), false, true);
					jumpType = EJumpType::EJT_wall;
					jumpPowerValue = 1.f;
				}
			}
			// # drop down from wallhug if down pressed
			else {
				jumpCounter = 0;
				bIsAtWallJump = false;
				bIsFalling = true;
				if (jumpDirection.Y > 0) {
					face = "R";
					jumpType = EJumpType::EJT_clean;
				}
				else if (jumpDirection.Y < 0) {
					face = "L";
					jumpType = EJumpType::EJT_clean;
				}
			}
		}

		// # jump
		else if (!bDownPressed || bIsOnGround) {
			if (JumpCurrentCount < jumpCounterMax && bIsOnGround) {
				//jumpCounter++;			// for now increased in BP, till sound comes from cpp
				LaunchCharacter(FVector(0, 0, doubleJumpLaunchPowerZ * jumpPowerValue), false, true);
				bIsJumping = true;
				jumpType = EJumpType::EJT_single;
				jumpPowerValue = 1.f;
			}
			// # double jump
			else {
				if (!bIsAtWallJump && jumpCounter < jumpCounterMax) {
					//jumpCounter++;
					LaunchCharacter(FVector(0, 0, doubleJumpLaunchPowerZ), false, true);
					bIsJumping = true;
					jumpType = EJumpType::EJT_double;
				}
				
			}
		}
		else if (bDownPressed && skills_CanStomp) {
			// stomp!
			if (bIsJumping) {
				if (stompCoolDown) {
					bDoStomp = true;
				}
			}

		}
	}
}

void AUnholyCharacter::AdjustMovementSpeed() {
	if (!bAllowMovement) return;

	else {
		// character velocity vars
		charVelocityX = GetVelocity().X;
		charVelocityY = GetVelocity().Y;
		charVelocityZ = GetVelocity().Z;

		// if walking right
		if (inputValueX >= 0) {
			if (face == "R") {
				adjustedMovementSpeed = maxMovementSpeed * runningSpeedModifier;
			}
			else if (face == "L") {
				adjustedMovementSpeed = maxMovementSpeed * backwardsSpeedModifier;
			}
		}
		// if walking left
		else if (inputValueX <= 0) {
			if (face == "L") {
				adjustedMovementSpeed = maxMovementSpeed * runningSpeedModifier;
			}
			else if (face == "R") {
				adjustedMovementSpeed = maxMovementSpeed * backwardsSpeedModifier;
			}
		}
		GetCharacterMovement()->MaxWalkSpeed = adjustedMovementSpeed;
	}
	
}

void AUnholyCharacter::MoveRight(float Value) {
	// break conditions
	if (!bAllowMovement) return;
	if (bIsAtWallJump) return;
	if (bIsCharging) return;

	else {
		inputValueX = Value;
		AddMovementInput(FVector(0.f, -1.f, 0.f), inputValueX);
	}
}

float AUnholyCharacter::GetSpeed(int axis) {
	FVector velo = GetVelocity();
	float speed = 0;  // negative Y value walks Right, positive walks left
	switch(axis) {
		case 1:
			// x-axis, no idea how you got here...
			if (face == "R") {
				speed = velo.X *-1;
			}
			else if (face == "L") {
				speed = velo.X;
			}
			break;
		case 2:
			// y-axis, normal movement
			if (face == "R") {
				speed = velo.Y  *-1;
			}
			else if (face == "L") {
				speed = velo.Y;
			}
			break;
		case 3:
			// z-axis; + jump / - fall
			speed = velo.Z;
			break;
	}
	return speed;
}

void AUnholyCharacter::FireWeapons() {
	// no auto fire, bugger off noob!
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "PEW!");
}

void AUnholyCharacter::FireCharged() {
	if (bCanChargeFire && skills_CanChargeFire) {
		bIsCharging = true;
	}
}

void AUnholyCharacter::FireChargeActivated() {
	if (chargeActiveTimerValue >= chargeActiveTimerMax) {
		bIsChargeFiring = true;						// trigger anim, reset in BP
		bCanChargeFire = false;
		chargeFireTimerValue = 0.f;
	}
	chargeActiveTimerValue = 0.f;
	bIsCharging = false;

}

void AUnholyCharacter::Use() {
	if (!bAllowMovement) return;

	else {
		if (bUsableInRange) {
			bUsableActivated = true;
		}
	}
}

void AUnholyCharacter::DownPressed() {
	bDownPressed = true;
}

void AUnholyCharacter::DownReleased() {
	bDownPressed = false;
}

void AUnholyCharacter::DealDamage(float dmg) {
	// check if shield is available
	if (shieldChargesValue > 0) {
		bShieldActive = true;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "shield active!");
		shieldChargesValue -= 1;
		timeSinceLastShieldHitValue = .0f;
	}
	else {
		if (healthValue > dmg) {
			healthValue -= dmg;
			bPlayHitSound = true;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::SanitizeFloat(dmg) + " dmg taken");
		}
		else {
			healthValue = 0.f;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "DIE!DIE!DIE!");
		}
	}

}

void AUnholyCharacter::IncreaseSkillsByLevel() {
	healthIncreased = healthMax + (skillLevel_health * skillValue_health);
	dashCDDecreased = dashCDMax - (skillLevel_dashCD * skillValue_dashCD);
	// skillLevel_wallhugDuration;	skill_ValuewallhugDuration;
	stompCoolDownDecreased = stompCoolDownMax - (skillLevel_StompCD * skillValue_StompCD);
	chargeFireTimerMaxDecreased = chargeFireTimerMax - (skillLevel_chargeUpShotTime* skillValue_chargeUpShotTime);
	shieldRechargeTimerMaxDecreased = shieldRechargeTimerMax - (skillLevel_shieldRechargeTime * skillValue_shieldRechargeTime);
	timeSinceLastShieldHitMaxDecreased = timeSinceLastShieldHitMax -(skillLevel_shieldRechargeDuration * skillValue_shieldRechargeDuration);
	// skillLevel_gadgetCoolDown * skillValue_gadgetCoolDown
	// skillLevel_GadgetCapacity * skillValue_GadgetCapacity;

}